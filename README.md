# Camera_Sym.Bios
Implementation of Edge Panning in the project Sym.Bios

This is a function created to allow for edge panning in the project Sym.Bios.
Edge panning is that when the player moves the mouse to the edge of the screen, the camera moves that direction similar to RTS cameras.
The main difficulty for this was figuring that GetViewport returned the scaled viewport while GetMousePositionViewport does not.
So for example if the viewport is of size x1250 y700 the mouse would be something like x2000 y1200.
My solution was to get the viewportsize and scale it back.

Other issue was when to disable edge panning, in the editor window you can shift F1, simulate window or alt tab.
It was really annonying for edge panning to happen at that point so I needed to check for certain conditions where edge panning 
should not happen.

The final issue was just that many functions I was using in Unreal did not make any sense. GetViewport id scaled but 
GetMousePositionViewport is not. IsSimulateInEditorViewport to me implies that this is true when simulating in editor however it does 
the opposite. Get ViewportSize is a delegate that needs to take a FViewport* and uint32. This uint32 is not used and not explained in 
the comments what it is for.
