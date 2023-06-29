#ifndef NODE_H
#define NODE_H
#pragma once

#include <stdio.h>
#include <typeinfo>
#include <string>
#include <cstring>
#include <unordered_map>

#define SCENE_ITEM_NAME_MAX_SIZE 32
#define SCENE_ITEM_NAME_MAX_NUMS 2

namespace Node_registrations {
	template <typename T>
	class Node_Registration{
	public:
		Node_Registration(const char* id){
			Node::get().register_node(
				id,
				[]() { return static_cast<Node*>(new T()); }
			);
		}
	};
}

#define REGISTER_NODE(NODE, NODE_NAME)\
		namespace Node_registrations {\
			Node_registration<NODE> NODE(NODE_NAME);\
		}

class Node{

	// Variables
	//-----------
	protected:

		// LINKED LIST POINTERS
		Node 				*child;    // Points to the first node child-ed to this
		Node 				*sibling;  // Allows for multiple children to a single node. Points to next sibling
		Node 				*prev_sibling; // Points to previous sibling in the linked list.
		Node 				*parent;   // Parent node. For quick access when needing to move up the tree.
		Node 				*next;	// Points to the next node to be created for deconstructing everything so as not to lose a node somewhere in RAM
		static Node 		*last;	// Points to the last node created.
	
	private:
		// IDENTIFICATION STUFF

		unsigned int 		ID;
		bool 				active;
		static unsigned int incrementor;

		// node list
		typedef Node*(*Node_Generator)();
		std::unordered_map< char, Node_Generator > available_nodes;
		
	public:

		char 				name[SCENE_ITEM_NAME_MAX_SIZE];

		// Factory functions
		static Node& get();
		bool register_node( const char* node_name, const Node_Generator& generate );
		bool generate_node( const char* node_name );

		// SceneTree events
		void ready();
		void update( float delta_time );
		void enter_tree();
		void exit_tree();

		// Virtual Callbacks
		virtual void _ready(){};
		virtual void _update( float delta_time ){};
		virtual void _enter_tree(){};
		virtual void _exit_tree(){};

		// Node Wrangling
		void add_child( Node* new_child );
		Node* pop();

		Node* pop_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] );
		void delete_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] );
		Node* pop_child_by_index( unsigned int index, bool return_last_if_index_OOB = false );
		void delete_child_by_index( unsigned int index, bool return_last_if_index_OOB = false );
		Node* pop_child_by_ID( unsigned int check_ID );
		void delete_child_by_ID( unsigned int check_ID );

		void delete_floating_nodes();

		// Node Getters
		bool get_active();
		void set_active( bool active_ );
		unsigned int get_ID();
		Node* get_parent();
		Node* get_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] );
		Node* get_sibling_by_name( char sibling_name[SCENE_ITEM_NAME_MAX_SIZE] );
		Node* get_child_by_index(unsigned int index, bool return_last_if_index_OOB = false );
		Node* get_sibling_by_index(unsigned int index, bool return_last_if_index_OOB = false );
		Node* get_child_by_ID( unsigned int check_ID );
		Node* get_sibling_by_ID( unsigned int check_ID );
		Node* get_last_sibling();
		Node* get_last_node();

		// Node Setters/Getters
		void set_name( const char new_name[SCENE_ITEM_NAME_MAX_SIZE] );


		// Constructor/Destructor
		Node();
		virtual ~Node();

};

#endif