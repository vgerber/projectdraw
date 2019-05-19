#pragma once


struct Size {

	/**
	 * @brief Construct a new Size object
	 * 
	 * @param width 
	 * @param height 
	 * @param depth 
	 */
	Size(float width = 0.0f, float height = 0.0f, float depth = 0.0f);

	/**
	 * @brief Construct a new Size object
	 * 
	 * @param x 
	 * @param y 
	 * @param width 
	 * @param height 
	 */
	Size(float offsetX, float offsetY, float width, float height);

	/**
	 * @brief Construct a new Size object
	 * 
	 * @param offsetX 
	 * @param offsetY 
	 * @param offsetZ 
	 * @param width 
	 * @param height 
	 * @param depth 
	 */
	Size(float offsetX, float offsetY, float offsetZ, float width, float height, float depth);

	float x = 0.0f; //offset x
	float y = 0.0f; //offset y
	float z = 0.0f; //oofset z


	float width = 0.0f;
	float height = 0.0f;
	float depth = 0.0f;
};

class IBoundingBox {
	/**
	 * @brief Get the Size object
	 * 
	 * @return Size 
	 */
	virtual Size getSize() = 0;

	/**
	 * @brief 
	 * 
	 * @param size 
	 */
	virtual void scaleToSize(Size size) = 0;

	/**
	 * @brief 
	 * 
	 * @param width 
	 */
	virtual void scaleToWidth(float width) = 0;

	/**
	 * @brief 
	 * 
	 * @param height 
	 */
	virtual void scaleToHeight(float height) = 0;

	/**
	 * @brief 
	 * 
	 * @param depth 
	 */
	virtual void scaleToDepth(float depth) = 0;
};