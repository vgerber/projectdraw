#pragma once

#include "Core/Scene/Text/text.h"


class MultilineText : public Text {
public:
    MultilineText(Font font) : Text(font) {}

    /**
     * @brief Set the maximum pixel width
     * 
     * pixel width not text length
     * 
     * @param width 
     */
    void setMaxWidth(int width);

    /**
     * @brief Set the Line offset to the next line
     * 
     * @param height 
     */
    void setLineHeight(int height);

    /**
     * @brief Get the Line offset to next line
     * 
     * @return int 
     */
    int getLineHeight();

    /**
     * @brief Get the processed text as lines
     * 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getTextLines();

    void processText();
private:
    int maxWidth = 10;
    int lineHeight = 0;
    std::vector<std::string> textLines;

    /**
     * @brief Splits the text in new lines where length < maxLength
     * 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getNewLines(std::string text);    

    std::string trimLine(std::string line);

    std::vector<std::string> getWords(std::string line);

    virtual void callUpdate() override;
};