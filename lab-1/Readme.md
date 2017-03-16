Laboratory Work Nr.1
====================
Title
-----
Window. Window handling.
Introduction
------------
Main purposes of this lab work:

•understand Event-Driven Programming.

•study Win32 API.

What was implemented?
--------------------
•Created a Windows application

•In the middle of the window inserted the following text: "Done with Pride and Prejudice by Spatari Stanislav".

•On windows resize, text reflows and it stays in the middle of the window(vertically and horizontally)

•Added 2 buttons to window: one with default styles, one with custom styles (background, text color)

•Added 2 text elements to window: one with default styles, one with custom styles (background, text color, font, size)

•Implemented actions
When the button Change Title is clicked, the window title changes, at the same time if we click the change basic message the message from the middle of the screen changes.

Also changed the behaviour of all 3 top window buttons. Minimize button now has custom handle in WM_SYSCOMMAND, and instead of standard minimization now it prompts a messageBox that asks if we want to close the progmram, by pressing ok the program will close and window will be destroyed. Maximize button on the other hand moves the our window to another place on the screen and Close button just prompts a simple message telling telling that "This is not a quit button".

Creation steps of the app:
--------------------------
In the process of creating the program a great help was provide by the Charles Petzold's book- *"Programming Windows"*. During the development of the app, [http://msdn.microsoft.com](http://msdn.microsoft.com) was also of great usage.

Analysing the structure of the program, it can be divided in two main parts: WinMain function, which represents the entry point in the program and WinProc, which is the function that processes all messages sent or posted to all windows of the class. So,in WinMain were declared the main window,while in WinProc were created a list of messages as WM_CREATE, WM_PAINT, WM_DRAWITEM and others, that were used to generate buttons,text elements and dialog boxes. 

Conclusions:
--------------------------
I learned how the elements of the window are created, how their behavior can be changed. I worked with messages, understood how the elements and their actions are interconnected. This concepts can be applied not only in windows programming, but also in another systems of programming.