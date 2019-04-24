#include "multiline_text.h"

void MultilineText::setMaxWidth(int width) {
    this->maxWidth = width;
    callUpdate();
}

void MultilineText::setLineHeight(int height) {
    this->lineHeight = height;
    callUpdate();
}

int MultilineText::getLineHeight() {
    return lineHeight;
}

std::vector<std::string> MultilineText::getTextLines() {
    return textLines;
}

void MultilineText::processText() {
    textLines.clear();

    int lastLine = 0;
    for(int i = 0; i < text.size(); i++) {
        if(text[i] == '\n') {
            std::string line = text.substr(lastLine, i - lastLine);
            textLines.push_back(line);
            lastLine = i+1;
        }
        else if(i == text.size()-1) {
            std::string line = text.substr(lastLine, (i+1) - lastLine);
            textLines.push_back(line);
        }
    }

    //format text:
    //1. split into desired size
    //2. or trim whitespaces
    for(int i = 0; i < textLines.size(); i++) {
        std::string line = textLines[i];
        unsigned int lineLength = font.getTextWidth(line);

        if(lineLength > maxWidth) {
            std::vector<std::string> newTextLines = getNewLines(line);
            textLines.erase(textLines.begin() + i);
            textLines.insert(textLines.begin()+i, newTextLines.begin(), newTextLines.end());

            for(auto newLine : newTextLines) {
                lineLength = font.getTextWidth(newLine);
            }

            i += newTextLines.size() - 1;
        }
        else {
            line = trimLine(line);
            textLines[i] = line;
        }
    }
}

std::vector<std::string> MultilineText::getNewLines(std::string text) {
    std::vector<std::string> newLines;
    std::vector<std::string> words;

    words = getWords(text);
    
    int spaceWidth = font.getTextWidth(" ");
    int lineWidth = 0;
    std::string newLine = "";
    for(auto word : words) {
        int wordWidth = font.getTextWidth(word);

        //break line if it is greater than maximum text size
        //>0 is importent for cases where the word is bigger than maxWidth
        if((lineWidth + wordWidth) > maxWidth && lineWidth > 0) {
            //printf("Newline \n");
            newLines.push_back(newLine);
            newLine = "";
            lineWidth = 0;
        }

        //append word and size
        newLine += word;
        lineWidth += wordWidth;

        //append space and size
        if((lineWidth + spaceWidth <= maxWidth) && lineWidth > 0) {
            newLine += " ";
            lineWidth += spaceWidth;
        }        
    }

    //append the last line if it is no empty
    //appears when the last words are less then maxWidth
    if(lineWidth > 0) {
        newLines.push_back(newLine);

    }

    return newLines;
}

std::string MultilineText::trimLine(std::string line) {
    //trim left
    for(int i = 0; i < line.size(); i++) {
        if(line[i] != ' ') {
            line = line.substr(i, line.size() - i);
            break;
        }
    }
    //trim right
    for(int i = line.size()-1; i >= 0; i--) {
        if(!isspace(line[i])) {
            line = line.substr(0, i+1);
            break;
        }
    }
    return line;
}

std::vector<std::string> MultilineText::getWords(std::string line) {
    std::vector<std::string> words;
    line = trimLine(line);
    int wordStart = 0;
    for(int i = 0; i < line.size(); i++) {
        int wordLength = 0;
        if(line[i] == ' ') {         
            wordLength = i - wordStart;   
        }
        else if(i == line.size()-1) {
            wordLength = (i+1) - wordStart;
        }
        else {
            continue;
        } 


        std::string word = line.substr(wordStart, wordLength);
        words.push_back(word);
        wordStart = i+1;
    }
    return words;
}

void MultilineText::callUpdate() {
    processText();
    size.width = maxWidth;
    int textHeights = (font.getVerticalBounds().x - font.getVerticalBounds().y) * textLines.size();
    int lineHeights = lineHeight * textLines.size()-1;
    size.height = textHeights + lineHeights;

    Drawable::callUpdate();
}