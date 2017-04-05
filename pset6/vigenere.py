import cs50

def main():
    # Get inputs from user
    print("Key: ", end="")
    key = str.upper(cs50.get_string())
    print("Text: ", end="")
    text = str.upper(cs50.get_string())
    
    # Initialise cipher variable
    cipher = ""
    
    for i in text:
        if i.isalpha() == True:
            c = ord(i) + (key % 26)
            while c > 90:
                c -= 26
            while c < 64:
                c += 26
            cipher += chr(c)
        else:
            cipher += i

    print(cipher)

if __name__ == "__main__":
    main()