import nltk
from nltk.tokenize import TweetTokenizer
s = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit."
tknzr = TweetTokenizer()
tokens = tknzr.tokenize(s)
tokens = [str.lower() for str in tokens]
print(tokens[1])
print(tokens)