// Globally accessible classes/functions go here
#include "node.hpp"
#include <cstring>



unsigned int node::incrementor = 0;
node *node::last = NULL;
//----------------------
//        NODE
//----------------------
// The Node type that will populate the SceneTree.


// SceneTree events
//------------------

// called when the node and all its subtree are built
void node::ready(){
	if( child ) 		child->ready();
	if( sibling )		sibling->ready();
	_ready();
}
// called once every game tick
void node::update( float delta_time ){
	_update( delta_time );
	if( !get_active() )	return;
	if( child )			child->update( delta_time );
	if( sibling )		sibling->update( delta_time );
}
// called when the node is added into the tree
void node::enter_tree(){
	_enter_tree();
	if( child )			child->enter_tree();
	if( sibling )		sibling->enter_tree();
}
// called when the node is removed from the tree
void node::exit_tree(){
	_exit_tree();
	if( child )			child->exit_tree();
	if( sibling )		sibling->exit_tree();
}


// Node Wrangling
//----------------

// Add child node to the top of the child list.
void node::add_child( node* new_child ){
	if( new_child->parent ) return;
	if( child ){
		node *temp_child	= child;
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

// Pop node from tree and link siblings together
node* node::pop(){
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
node* node::pop_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !child ) return NULL;
	return get_child_by_name( child_name )->pop();
}
void node::delete_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !child ) return;
	delete pop_child_by_name( child_name );
}
node* node::pop_child_by_index( unsigned int index, bool return_last_if_index_OOB ){
	if( !child ) return NULL;
	return get_child_by_index( index, return_last_if_index_OOB )->pop();
}
void node::delete_child_by_index( unsigned int index, bool return_last_if_index_OOB ){
	if( !child ) return;
	delete pop_child_by_index( index,return_last_if_index_OOB );
}
node* node::pop_child_by_ID( unsigned int check_ID ){
	if( !child ) return NULL;
	return get_child_by_ID( check_ID )->pop();
}
void node::delete_child_by_ID( unsigned int check_ID ){
	if( !child ) return;
	delete pop_child_by_ID( check_ID );
}

// Removes nodes that don't have a parent and thus aren't in the tree
void node::delete_floating_nodes(){
	if( !next ) return;
	node *temp_node_ptr = next;
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

bool node::get_active(){
	return active;
}
void node::set_active( bool active_ ){
	active = active_;
}
unsigned int node::get_ID(){
	return ID;
}
node* node::get_parent(){
	return parent;
}
node* node::get_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !child ) return NULL;
	if( child->name == child_name ) return child;
	return child->get_sibling_by_name( child_name );
}
node* node::get_sibling_by_name( char sibling_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	if( !sibling ) return NULL;
	if( sibling->name == sibling_name ) return sibling;
	return sibling->get_sibling_by_name( sibling_name );
}
node* node::get_child_by_index(unsigned int index, bool return_last_if_index_OOB ){
	if( !child ) return NULL;
	return child->get_sibling_by_index( index--, return_last_if_index_OOB );
}
node* node::get_sibling_by_index(unsigned int index, bool return_last_if_index_OOB ){
	if( index == 0 ) return this;
	if( sibling ){
		return sibling->get_sibling_by_index( index--, return_last_if_index_OOB );
	}else if( return_last_if_index_OOB ){
		return this;
	}else{
		return NULL;
	}
}
node* node::get_child_by_ID( unsigned int check_ID ){
	if( !child ) return NULL;
	if( child->get_ID() == check_ID ) return child;
	return child->get_sibling_by_ID( check_ID );
}
node* node::get_sibling_by_ID( unsigned int check_ID ){
	if( get_ID() == check_ID ) return this;
	if( sibling ) return sibling->get_sibling_by_ID( check_ID );
	return NULL;
}
node* node::get_last_sibling(){
	if( !sibling ) return this;
	return sibling->get_last_sibling();
}
node* node::get_last_node(){
	if( !next ){ return this;};
	return next->get_last_node();
}


// Node Setters
//--------------

void node::set_name( const char new_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	strcpy( name, new_name );
}

// Constructor/Destructor
//------------------------

node::node( char new_name[SCENE_ITEM_NAME_MAX_SIZE] ){
	ID = incrementor++;
	active = true;
	child = NULL;
	sibling = NULL;
	prev_sibling = NULL;
	parent = NULL;
	next = NULL;
	snprintf( name, sizeof(name), "%u", get_ID() );
	set_name(new_name);
	if( last ) last->next = this;
	last = this;
}
node::~node(){
	if( child ) delete child;
	if( sibling ) delete sibling;
}

/*
// Variables
char node::name[SCENE_ITEM_NAME_MAX_SIZE];

// LINKED LIST POINTERS
node 		*node::child;    // points to the first node child-ed to this
node 		*node::sibling;  // allows for multiple children to a single node
node 		*node::prev_sibling;
node 		*node::parent;   // for quick access when needing to move up the tree
node 		*node::next;	// points to the next node to be created for deconstructing everything so as not to lose a node somewhere in RAM
static node *node::last;

unsigned int 		node::ID;
bool 				node::active;
static unsigned int node::incrementor;*/