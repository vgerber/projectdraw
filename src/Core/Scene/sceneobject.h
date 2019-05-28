#pragma once

#include <string>
#include <algorithm>
#include <functional>

#include "Core/Util/Transform/moveable.h"

class SceneObject : public Moveable
{
public:
	SceneObject();
	~SceneObject();

	/**
	 * @brief Get child by id
	 * 
	 * @param id 
	 * @return SceneObject* 
	 */
	SceneObject * getChild(std::string id);

	/**
	 * @brief Add child to current node
	 * 
	 * @param sceneObject 
	 */
	void addChild(SceneObject * sceneObject);

	/**
	 * @brief Get the children
	 * 
	 * @return std::vector<SceneObject*> 
	 */
	std::vector<SceneObject*> getChildren() const;

	/**
	 * @brief Removes child node from tree
	 * 
	 * The current node where this function has been called will be the root node for this operation
	 * 
	 * Remove will collapse subchildren to root node if full is false 
	 * otherwhise subchildren will be removed to
	 * 
	 * @param sceneObject SceneObject which will be removed
	 * @param full True if all childs from the target will be removed too
	 */
	void removeChild(SceneObject * sceneObject);

	void removeAllChildren();

	/**
	 * @brief Get the object id
	 * 
	 * @return std::string 
	 */
	std::string getId() const;

	/**
	 * @brief Set the object id
	 * 
	 * @param id Name for object
	 */
	void setId(std::string id);
	
	/**
	 * @brief Get the world transform
	 * 
	 * @return Transform 
	 */
	Transform getWorldTransform() const;

	/**
	 * @brief Prevents calling update after each change
	 * 
	 * Useful for long editing routines 
	 * 
	 */
	void beginEdit();

	/**
	 * @brief Enables update calls and calls update
	 * 
	 */
	void endEdit();

	/**
	 * @brief Adds listener to receiver
	 * 
	 * Notifies receiver when data has changed
	 * 
	 * @param receiver 
	 * @param receiverFunction 
	 */
	void addUpdateListener(void * receiver, std::function<void()> receiverFunction);

	/**
	 * @brief Removes listener from receiver
	 * 
	 * @param receiver 
	 */
	void removeUpdateListener(void * receiver);

	/**
	 * @brief Adds listener to receiver
	 * 
	 * Notifies receiver when receiver when tree structer has been changed
	 * 
	 * @param receiver 
	 * @param receiverFunction 
	 */
	void addUpdateTreeListener(void * receiver, std::function<void(SceneObject*)> receiverFunction);

	/**
	 * @brief Removes listener from receiver
	 * 
	 * @param receiver 
	 */
	void removeUpdateTreeListener(void * receiver);

	/**
	 * @brief Returns true if object is in edit mode
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isEditMode();

protected:
	//object id/name
	std::string id;

	//storage of children
	std::vector<SceneObject*> children;

	///Transform from root to parent
	Transform parentTransform;

	/**
	 * @brief Will be called when object transform has been changed
	 * 
	 * Children get a @ref parentTransformChanged call
	 */
	virtual void transformChanged() override;

	/**
	 * @brief Called when parents transform has been changed
	 * 
	 * @param transform 
	 */
	virtual void parentTransformChanged(Transform transform);

	/**
	 * @brief Notifies all receivers
	 * 
	 */
	virtual void callUpdate();

	/**
	 * @brief Notifies all receivers if tree structer has been changed
	 * 
	 */
	virtual void callUpdateTree();
private:
	///Cached transform with parent, base and model transform
	///Refreshed in each transformChnaged() and parenTransformChanged()
	Transform cachedWorldTransform;

	std::vector<std::pair<void*, std::function<void()>>> updateListeners;
	std::vector<std::pair<void*, std::function<void(SceneObject*)>>> updateTreeListeners;

	bool editMode = false;

};

