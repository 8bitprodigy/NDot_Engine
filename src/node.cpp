// Globally accessible classes/functions go here
#include "node.hpp"
#include <cstring>



unsigned int Node::incrementor = 0;
Node *Node::last = NULL;
//----------------------
//        NODE
//----------------------
// The Node type that will populate the SceneTree.


// Factory Management
//--------------------

Node & Node::get(){
	static Node instance;
	return instance;
}

bool Node::register_node( 
		const char* type_name, 
		const Node_Generator & generator_function
	){
	return available_nodes.insert(std::make_pair(type_name, generator_function)).second;
}

// SceneTree events
//------------------

// called when the node and all its subtree are built
void Node::ready(){
	if( child ) 		child->ready();
	if( sibling )		sibling->ready();
	_ready();
}
// called once every game tick
void Node::update( float delta_time ){
	_update( delta_time );
	if( !get_active() )	return;
	if( child )			child->update( delta_time );
	if( sibling )		sibling->update( delta_time );
}
// called when the node is added into the tree
void Node::enter_tree(){
	_enter_tree();
	if( child )			child->enter_tree();
	if( sibling )		sibling->enter_tree();
}
// called when the node is removed from the tree
void Node::exit_tree(){
	_exit_tree();
	if( child )			child->exit_tree();
	if( sibling )		sibling->exit_tree();
}


// Node Wrangling
//----------------

// Add child node to the top of the child list.
void Node::add_child( Node* new_child ){
	if( new_child->parent ) return;
	if( child ){
		if(get_child_by_name(new_child->name)) snprintf( new_child->name, sizeof(new_child->name), "%u", new_child->get_ID() );
		Node *temp_child	= child;
		new_child->sibling	= temp_child;
		temp_child->prev_sibling = new_child;
		/*char end_chars[SCENE_ITEM_NAME_MAX_NUMS] = new_child->name[strlen(new_child->name)-SCENE_ITEM_NAME_MAX_NUMS];
		for(int i=0;i<SCENE_ITEM_NAME_MAX_NUMS;i++){
			if(!(isdigit(end_chars[i]))){
				break;
			}
		}*/
	}
	child = new_child;
	//if(child) child->get_last_sibling()->sibling = new_child;
	//else child = new_child;
	new_child->parent = this;
	new_child->enter_tree();
}

/*
// So loading is a static function which doesn't take any instance yet and creates one
node* node::load(FILE* pFile) { 
    // need to read in a name before we can create a node
    char name[256];
    fscanf(pFile, "%s", name);
    Node* pNode = new Node(name);
    
    // now read in other shit a node has
    int x, y;
    fscanf(pFile, "%d%d", &x, &y);
    pNode->setPos(x, y);

    // now read in # of children
    unsigned nodeCount = 0;
    fscanf(pFile, "%u", &nodeCount);

    for(unsigned n = 0; n < nodeCount; ++n) {
        // create a new node for every child, 
        // and add it to this node
        pNode->addChild(Node::load(pFile)); 
    }

    // return our new node
    return pNode;
}*/

// Pop node from tree and link siblings together
Node* Node::pop(){
	if( sibling ){
		if( prev_sibling ){
			prev_sibling->sibling		= sibling;
			sibling->prev_sibling		= prev_sibling;
		} else sibling->prev_sibling	= NULL;
	}else prev_sibling->sibling	= NULL;
	prev_sibling	= NULL;
	//next = NULL;
	return this;
}

// Helper Functions
Node* Node::pop_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !child ) return NULL;
	return get_child_by_name( child_name )->pop();
}
void Node::delete_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !child ) return;
	delete pop_child_by_name( child_name );
}
Node* Node::pop_child_by_index( unsigned int index, bool return_last_if_index_OOB ){
	if( !child ) return NULL;
	return get_child_by_index( index, return_last_if_index_OOB )->pop();
}
void Node::delete_child_by_index( unsigned int index, bool return_last_if_index_OOB ){
	if( !child ) return;
	delete pop_child_by_index( index, return_last_if_index_OOB );
}
Node* Node::pop_child_by_ID( unsigned int check_ID ){
	if( !child ) return NULL;
	return get_child_by_ID( check_ID )->pop();
}
void Node::delete_child_by_ID( unsigned int check_ID ){
	if( !child ) return;
	delete pop_child_by_ID( check_ID );
}

// Removes nodes that don't have a parent and thus aren't in the tree
void Node::delete_floating_nodes(){
	if( !next ) return;
	Node *temp_node_ptr = next;
	if( temp_node_ptr->parent ){
		next->delete_floating_nodes();
		return;
	}
	if( temp_node_ptr->next ){
		next = temp_node_ptr->next;
	}else{
		last = this;
	}
	delete temp_node_ptr;
	delete_floating_nodes();
}


// Node Getters
//--------------

bool Node::get_active(){
	return active;
}
void Node::set_active( bool active_ ){
	active = active_;
}
unsigned int Node::get_ID(){
	return ID;
}
Node* Node::get_parent(){
	return parent;
}
Node* Node::get_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !child ) return NULL;
	if( child->name == child_name ) return child;
	return child->get_sibling_by_name( child_name );
}
Node* Node::get_sibling_by_name( char sibling_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !sibling ) return NULL;
	if( sibling->name == sibling_name ) return sibling;
	return sibling->get_sibling_by_name( sibling_name );
}
Node* Node::get_child_by_index(unsigned int index, bool return_last_if_index_OOB ){
	if( !child ) return NULL;
	return child->get_sibling_by_index( index--, return_last_if_index_OOB );
}
Node* Node::get_sibling_by_index(unsigned int index, bool return_last_if_index_OOB ){
	if( index == 0 ) return this;
	if( sibling ){
		return sibling->get_sibling_by_index( index--, return_last_if_index_OOB );
	}else if( return_last_if_index_OOB ){
		return this;
	}else{
		return NULL;
	}
}
Node* Node::get_child_by_ID( unsigned int check_ID ){
	if( !child ) return NULL;
	if( child->get_ID() == check_ID ) return child;
	return child->get_sibling_by_ID( check_ID );
}
Node* Node::get_sibling_by_ID( unsigned int check_ID ){
	if( get_ID() == check_ID ) return this;
	if( sibling ) return sibling->get_sibling_by_ID( check_ID );
	return NULL;
}
Node* Node::get_last_sibling(){
	if( !sibling ) return this;
	return sibling->get_last_sibling();
}
Node* Node::get_last_node(){
	if( !next ){ return this;};
	return next->get_last_node();
}


// Node Setters
//--------------

void Node::set_name( const char new_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	strcpy( name, new_name );
}

// Constructor/Destructor
//------------------------

Node::Node(){
	ID = incrementor++;
	active = true;
	child = NULL;
	sibling = NULL;
	prev_sibling = NULL;
	parent = NULL;
	next = NULL;
	strcpy(name,"Node");
	//printf(typeid(this).name());
	//snprintf( name, sizeof(name), "%u", get_ID() );
	//set_name(new_name);
	if( last ) last->next = this;
	last = this;
}
Node::~Node(){
	if( child ) delete child;
	if( sibling ) delete sibling;
}
