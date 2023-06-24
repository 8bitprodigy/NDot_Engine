#ifndef NODE_H
#define NODE_H
#pragma once

#include <stdio.h>
#include <typeinfo>

#define SCENE_ITEM_NAME_MAX_SIZE 32
#define SCENE_ITEM_NAME_MAX_NUMS 2

class node{

	// Variables
	//-----------
	protected:

		// LINKED LIST POINTERS
		node 				*child;    // points to the first node child-ed to this
		node 				*sibling;  // allows for multiple children to a single node
		node 				*prev_sibling;
		node 				*parent;   // for quick access when needing to move up the tree
		node 				*next;	// points to the next node to be created for deconstructing everything so as not to lose a node somewhere in RAM
		static node 		*last;
	
	private:
		// IDENTIFICATION STUFF
		unsigned int 		ID;
		bool 				active;
		static unsigned int incrementor;
		
	public:

		char 				name[SCENE_ITEM_NAME_MAX_SIZE];

		// SceneTree events
		void ready();
		void update( float delta_time );
		void enter_tree();
		void exit_tree();

		// Virtual Callbacks
		virtual void _ready(){};
		virtual void _update(float delta_time){};
		virtual void _enter_tree(){};
		virtual void _exit_tree(){};

		// Node Wrangling
		void add_child( node* new_child );
		node* pop();

		node* pop_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] );
		void delete_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] );
		node* pop_child_by_index( unsigned int index, bool return_last_if_index_OOB = false );
		void delete_child_by_index( unsigned int index, bool return_last_if_index_OOB = false );
		node* pop_child_by_ID( unsigned int check_ID );
		void delete_child_by_ID( unsigned int check_ID );

		void delete_floating_nodes();

		// Node Getters
		bool get_active();
		void set_active( bool active_ );
		unsigned int get_ID();
		node* get_parent();
		node* get_child_by_name( char child_name[SCENE_ITEM_NAME_MAX_SIZE] );
		node* get_sibling_by_name( char sibling_name[SCENE_ITEM_NAME_MAX_SIZE] );
		node* get_child_by_index(unsigned int index, bool return_last_if_index_OOB = false );
		node* get_sibling_by_index(unsigned int index, bool return_last_if_index_OOB = false );
		node* get_child_by_ID( unsigned int check_ID );
		node* get_sibling_by_ID( unsigned int check_ID );
		node* get_last_sibling();
		node* get_last_node();

		// Node Setters/Getters
		void set_name( const char new_name[SCENE_ITEM_NAME_MAX_SIZE] );


		// Constructor/Destructor
		node();
		virtual ~node();

};

#endif