#pragma once
#include <osg/Referenced>
#include <list>

using namespace std;
namespace osgForm{
	class TreeNode;

	typedef list<TreeNode*> List;
	//TreeNode中的ID由用户指定。
	class COM_EXPORT TreeNode 
	{
		public:
			TreeNode(string,int,TreeNode*);
			~TreeNode();
		
			void setParent(TreeNode&); 
			bool insertChildren(TreeNode&);
			
			string		_label;
			int			_ID;
			List		_children;
			TreeNode*	_parent;
	};
	class COM_EXPORT Tree 
	{
	public:
		Tree(void);
		~Tree(void);
		void insertTreeNode(TreeNode&);
		//TreeNode* insertTreeNode(string);
		bool isRoot(TreeNode*);
		bool isEmpty();
		bool isLeaf(TreeNode*);
		int getRootNum();
		TreeNode* getParent(TreeNode*);
		int getNumChildren(TreeNode*);
		List getChileren(TreeNode*);
		TreeNode* getNodeFromID(int);
		List getRoots(){return _nodes;}
		void clone(Tree* tree);
		void cloneTreeNode(TreeNode* parent,TreeNode* node);
	private:
		TreeNode* Finded(int,List);
	private:
		List		_nodes;
	};
}