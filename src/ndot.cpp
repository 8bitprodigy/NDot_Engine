#include "node.hpp"
#include <libdragon.h>
#include <cstdio>


class foo_node: public Node {
	public:
		void _update(float delta_time){
			printf("Foo!\t | Delta Time: %2.20f\n", delta_time);
			printf("\t"); printf(name); printf("\n");
			if(parent){ printf("\tParent: "); printf(parent->name);printf("\n"); }
			if(sibling){ printf("\tSibling: "); printf(sibling->name);printf("\n"); }
			if(child){ printf("\tChild: "); printf(child->name); printf("\n\n");}
		}
		foo_node() : Node(){
			strcpy(name, "Foo");
			//snprintf( name, sizeof(name)+4, "%u", get_ID() );
		}
};

class bar_node: public Node {
	public:
		void _update(float delta_time){
			printf("Bar!\n");
			printf("\t"); printf(name); printf("\n");
			if(parent){ printf("\tParent: "); printf(parent->name);printf("\n"); }
			if(sibling){ printf("\tSibling: "); printf(sibling->name);printf("\n"); }
			if(child){ printf("\tChild: "); printf(child->name); printf("\n\n");}
		}
		bar_node() : Node(){
			strcpy(name, "Bar");
			//snprintf( name, sizeof(name)+4, "%u", get_ID() );
		}
};


class foobar_node: public Node {
	public:
		void _update(float delta_time){
			printf("Foo-Bar!\n");
			printf("\t"); printf(name); printf("\n");
			if(parent){ printf("\tParent: "); printf(parent->name);printf("\n"); }
			if(sibling){ printf("\tSibling: "); printf(sibling->name);printf("\n"); }
			if(child){ printf("\tChild: "); printf(child->name); printf("\n\n");}
		}
		foobar_node() : Node(){
			strcpy(name, "FooBar");
			//snprintf( name, sizeof(name)+4, "%u", get_ID() );
		}
};


class N {
	public:
		Node *scene_root;

		void init(){
			console_init();
			timer_init();
			controller_init();
    		console_set_render_mode(RENDER_MANUAL);
		}
		void run(){
			float prev_time = 0.0f;
			float new_time = 0.0f;
			while(1){
				new_time = (float) timer_ticks() / TICKS_PER_SECOND;
				controller_scan();
				if(scene_root){
					console_clear();
					scene_root->update(new_time - prev_time);
					console_render();
				}
				prev_time = new_time;
			}
		}
		void engine(){
			init();
			run();
		}

		N(){

		}
		~N(){
			delete scene_root;
		}
};

// To be moved to a main.cpp file later
int main(void) {
	
	Node *root = new Node();
	foo_node *foo = new foo_node();
	bar_node *bar = new bar_node();
	foobar_node *foobar = new foobar_node();

	foobar = dynamic_cast<foobar_node*>(foobar);
	
	root->add_child(bar);
	root->add_child(foo);
	bar->add_child(foobar);

	N n;
	n.scene_root = root;
	n.engine();
}