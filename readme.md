CDHERE
======

`cdhere` is a Windows command-line program that changes the current directory 
to whatever directory the topmost Explorer window is looking at. In other 
words, it is the reverse of the many "[Open][1] [Command][2] [Window][3] 
[Here][4]" tricks that exist for various versions of Windows. Instead of
opening a new command window for each directory in which you want to do work.
you move the current directory in an existing console to where you want it to
be.

Installation
------------

* Download <https://github.com/eteeselink/cdhere/raw/master/Downloads/cdhere-1.0.zip>
  and unzip it somewhere convenient.
* Run `addToPath.cmd` with administrator priviledges to add the directory to
  your PATH, so that you can acces `cdhere` from anywhere.

Usage
-----
    
    cdhere

Which looks like:
<!-- my apologies to peolpe reading the below in a text editor; it's not very markdownish of me, really-->
<pre>
C:\Users\teeselinke>cdhere
D:\hobby\cdhere\cdhere\src><blink>▮</blink>
</pre>

Background
----------

Windows processes cannot change the directory of their parent process, but
batch files can. Therefore, `cdhere` consists of two files, a C++ program
called `cdwhere.exe` which finds the topmost Explorer window and queries it
for its current directory, and `cdhere.cmd`, which uses `cdwhere.exe`'s 
output to actually change the directory.

Bugs
----

Please report bugs using GitHub's issue tracker at
<https://github.com/eteeselink/cdhere/issues>

Acknowledgements
----------------

The only not-entirely-trivial bit of this code is the part that asks an 
Explorer window where it's pointed. This, however, is something that Raymond 
Chen (of [The Old New Thing][5] fame) already worked out for me [back in 2004]
[6]. Much of the code you see here is little more than his example refactored.


[1]: http://www.mydigitallife.info/add-open-command-window-here-to-xp-folder-with-powertoy/
[2]: http://stackoverflow.com/a/1225574/103395
[3]: http://code.kliu.org/cmdopen/
[4]: http://go.microsoft.com/fwlink/?LinkId=211417
[5]: http://blogs.msdn.com/b/oldnewthing
[6]: http://blogs.msdn.com/b/oldnewthing/archive/2004/07/20/188696.aspx