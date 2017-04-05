import nltk
from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = set()
        self.negatives = set()
        
        # For each line in positive doc, remove white space and add each non-comment line
        for line in open('positive-words.txt'):
            word = line.strip()
            if not line.startswith(';') and not line.startswith('\n'):
                self.positives.add(word)
                
        # For each line in negative doc, remove white space and add each non-comment line
        for line in open('negative-words.txt'):
            word = line.strip()
            if not line.startswith(';') and not line.startswith('\n'):
                self.negatives.add(word)


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # Separate text into list of individual words and make lower-case
        tokenizer = TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        tokens = [str.lower() for str in tokens]
        
        # For each token, check if in self.positives or self.negatives
        score = 0
        for i in range(len(tokens)):
            if tokens[i] in self.positives:
                score += 1
            elif tokens[i] in self.negatives:
                score -= 1
        return score
