from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from datetime import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Display user's stock portfolio."""

    # obtain cash info from users database
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    grandtotal = cash[0]["cash"]
    
    # obtain stock info from portfolio database
    stocks = db.execute("SELECT symbol, shares FROM portfolio WHERE id = :id", id=session["user_id"])
    
    # for every stock in the user's portfolio, assign dict key/values for use in html/jinja
    for stock in stocks:
        symbol = str(stock["symbol"])
        shares = int(stock["shares"])
        name = ""
        price = ""
        total = ""
        quote = lookup(symbol)
        stock["name"] = quote["name"]
        stock["price"] = "{:.2f}".format(quote["price"])
        stock["total"] = "{:.2f}".format(quote["price"] * shares)
        stock["grandtotal"] = quote["price"] * shares
        grandtotal += stock["grandtotal"]

    # format grandtotal to force 2 decimal places
    grandtotal = "{:.2f}".format(grandtotal)
    
    # render index page with some given values
    return render_template("index.html", stocks = stocks, cash = cash, grandtotal = grandtotal)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via POST, check all fields are filled
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("must provide symbol and number of shares")
        
        # use lookup function to get stock info
        quote = lookup(request.form.get("symbol"))
        
        # ensure validity of form
        if quote == None:
            return apology("invalid symbol")
        if not request.form.get("shares").isdigit():
            return apology("must provide positive integer")
        shares = int(request.form.get("shares"))
        price = round(float(quote["price"]),2)
        if shares < 1:
            return apology("must provide a positive integer of shares")
            
        # compare user's cash amount to calculated cost of shares
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        cost = round(float(shares * price),2)
        
        # return error if not enough cash
        if cost > cash[0]["cash"]:
            return apology ("insufficient funds")
        
        # if sufficient cash present, update users, portfolio and history tables with new info
        else:
            db.execute("UPDATE users SET cash = cash - :cost WHERE id = :id", cost=cost, id = session["user_id"])
            db.execute("UPDATE portfolio SET shares = shares + :shares WHERE id = :id AND symbol = :symbol", id=session["user_id"],symbol=quote["symbol"],shares=shares)
            db.execute("INSERT OR IGNORE INTO portfolio (id,symbol,shares) VALUES (:id,:symbol,:shares)",id=session["user_id"],symbol=quote["symbol"],shares=shares)
            db.execute("INSERT INTO history (id,symbol,shares,price,date) VALUES (:id,:symbol,:shares,:price,datetime('now'))",id=session["user_id"], symbol=quote["symbol"],shares=shares,price=price)
        
        flash('Bought!')
        return redirect(url_for("index"))
        
    # else if user reached route via GET
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # obtain stock info from portfolio database
    history = db.execute("SELECT symbol, shares, price, date FROM history WHERE id = :id ORDER BY date DESC", id=session["user_id"])
    
    # for every stock in the user's portfolio, assign dict key/values for use in html/jinja
    for transaction in history:
        symbol = transaction["symbol"]
        shares = transaction["shares"]
        price = transaction["price"]
        date = transaction["date"]

    return render_template("history.html", history = history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        flash('Successfully logged in!')
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via GET, return quote page
    if request.method == "GET":
        return render_template("quote.html")
        
    # if user reached via POST i.e. submitting form, check that the form is valid
    elif request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        
        # use lookup function to get current stock info, given its symbol
        quote = lookup(request.form.get("symbol"))
        
        # check stock exists
        if quote == None:
            return apology("invalid symbol")
        
        # if it does, display info given by quote list
        else:
            return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=round(quote["price"],2))

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # if user reached route via GET, return register page
    if request.method == "GET":
        return render_template("register.html")
    
    # otherwise, if reached via POST i.e. submitting a form...
    elif request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
        
        # ensure password and confirmation are equal
        elif request.form.get("password") != request.form.get("confirmPassword"):
            return apology("password fields do not match")
            
        # encrypt password
        hash = pwd_context.encrypt(request.form.get("password"))

        # comment
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"),hash=hash)
        if not result:
            return apology("username already in use")
        
        # log user in
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        flash('Successfully registered!')
        return redirect(url_for("index"))

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    # if user reached route via POST, check all fields are filled
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("must provide symbol and number of shares")
        
        # use lookup function to get stock info
        quote = lookup(request.form.get("symbol"))
        
        # ensure validity of form
        if quote == None:
            return apology("invalid symbol")
        if not request.form.get("shares").isdigit():
            return apology("must provide positive integer")
        
        # initiate variables
        shares = int(request.form.get("shares"))
        stocks = []
        
        # obtain user's stock information from portfolio database
        stocks = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol = :symbol", id=session["user_id"], symbol = quote["symbol"])
        
        # check that user actually owns enough stock, or any stock at all
        if stocks == []:
            return apology("you don't own any of this stock")
        if shares > stocks[0]["shares"]:
            return apology("invalid number of shares")
            
        # calculate price per share and cost of all shares
        price = round(float(quote["price"]),2)
        cost = round(float(shares * price),2)
        
        # update user's cash balance
        db.execute("UPDATE users SET cash = cash + :cost WHERE id = :id", cost = cost, id=session["user_id"])
        
        # if there are still shares leftover after sale, update row
        if shares < stocks[0]["shares"]:
            db.execute("UPDATE portfolio SET shares = shares - :shares WHERE id = :id AND symbol = :symbol", id=session["user_id"], shares = shares, symbol = quote["symbol"])
        
        # otherwise, if not shares leftover, remove row from portfolio entirely
        elif shares == stocks[0]["shares"]:
            db.execute("DELETE FROM portfolio WHERE id = :id AND symbol = :symbol", id=session["user_id"], symbol = quote["symbol"])
            
        db.execute("INSERT INTO history (id,symbol,shares,price,date) VALUES (:id,:symbol,:shares,:price,datetime('now'))",id=session["user_id"], symbol=quote["symbol"],shares=-shares,price=price)
        
        flash('Sold!')
        return redirect(url_for("index"))
        
    # else if user reached route via GET
    else:
        return render_template("sell.html")

@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit cash into account."""
    
    # if user reached route via GET, return deposit page
    if request.method == "GET":
        return render_template("deposit.html")
        
    # if user reached via POST i.e. submitting form, check that the form is valid
    elif request.method == "POST":
        if not request.form.get("amount"):
            return apology("must provide amount")
        if not request.form.get("amount").isdigit():
            return apology("must provide positive integer")
        
        # update user's cash amount
        db.execute("UPDATE users SET cash = cash + :amount WHERE id = :id", amount = request.form.get("amount"), id = session["user_id"])
        flash("Successfully deposited cash!")
        return redirect(url_for("index"))
