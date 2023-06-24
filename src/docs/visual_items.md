All visual items, including both cameras, and renderable items will have access to a linked list of renderable items. Only the renderable items will be part of the list, however.

Visual items will also have an 8-bit value (likely a char) to hold a set of flags that represent what render layers these visual items are present in.

When a renderable item enters the scenetree, it will add itself to the render list to be rendered each frame, and when it exits t he scenetree, it removes itself from the list. Renderable items can also have their visibility turned on or off, which will also add or remove them from the render list.

A frame is rendered by a camera, which iterates through the render list, checks them to see if they're within the frustum and on the same render layer, and if so, calls their render method.

Here is what the Visual and Renderable Item classes should look something like this:

	class Visual_Item{
		char render_layer;
		static Visual_Item *head;
		//whatever common functions used by all Visual Items go here.
	}

	class Renderable_Item:Visual_Item{
		Renderable_Item *next;
		Renderable_Item *previous;

		virtual void _enter_tree(){
			next = head;
			head->previous = this;
			head = this;
		}
	}