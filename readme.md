# NBI
NBI is an interactive digital logic emulator built on SFML and various other libraries.

![NBI Demo](https://github.com/wvannoordt/nbi/blob/main/doc/img/displayimg.png?raw=true)

It is currently in the early stages of development and is therefore a little bit clunky and unoptimized.

# Installation

TODO!

# Usage

## The Basics

After building NBI, the executable file `bin/nbi` can be run to start the application. This will enter you onto a blank canvas.

![Blank canvas](https://github.com/wvannoordt/nbi/blob/main/doc/img/ini.png?raw=true)

You can move around the canvas by dragging the cursor with the right-click button held. Scrolling up
will zoom into the current cursor position, and scrolling down will zoom out. There isn't much to see yet,
so let's add some stuff!

## Adding Your First Logic Gate

Press `a` to enter gate add mode. A logic gate icon should follow your cursor.
![Add mode](https://github.com/wvannoordt/nbi/blob/main/doc/img/addmode.png?raw=true)

You can press `r` to rotate the gate 45 degrees clockwise. Pressing `shift + r` will rotate the other direction
![Rotate gate](https://github.com/wvannoordt/nbi/blob/main/doc/img/rotategate.png?raw=true)

By pressing `tab`, you can change the type of gate that you are adding. To keep things simple, there are only four gates:
`identity`, `inverse`, `or`, and `and`.
![Next gate](https://github.com/wvannoordt/nbi/blob/main/doc/img/nextgate.png?raw=true)

Click the left mouse button to add the gate to the canvas. You can now proceed to add other gates elsewhere!
![Add gate](https://github.com/wvannoordt/nbi/blob/main/doc/img/addgate.png?raw=true)

## Manipulating State

When you hover your cursor over a gate on the canvas, it will be partially highlighted.
![Highlight node](https://github.com/wvannoordt/nbi/blob/main/doc/img/nodehighlight.png?raw=true)

While the node is partially highlighted, you can right-click to toggle the node state.
![Node change 1](https://github.com/wvannoordt/nbi/blob/main/doc/img/nodehighchange1.png?raw=true)

The `on` state is indicated in blue, and the `off` state is indicated in grey. Red indicates an invalid
or uninitialized state.
![Node change 2](https://github.com/wvannoordt/nbi/blob/main/doc/img/nodehighchange2.png?raw=true)

## Creating Edges

While hovering over a node, you can select it by clicking it.
![Node select](https://github.com/wvannoordt/nbi/blob/main/doc/img/nodeselect.png?raw=true)

# License
[GNU GENERAL PUBLIC LICENSE](https://choosealicense.com/licenses/gpl-3.0/)
