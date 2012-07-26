# Len's Test Mission: Goals
***

### Creating an OpenNI-based background service.
> _You have to provide a background-task service that captures Asus Xtion Pro Live stream and detects left and right hands coordinates. These coordinates shall be continuously readable through a localhost socket, as fast as possible._
> _You are free to choose any third-party SDK you prefer to work with to do hands recognition._
> _Just make simple and clean code, and don't forget documentation is often more important than code._

***

### Compilable Programming Language.
> _You have the choice of the programming language, as long as it can be compiled (no script-only) and satisfy the cross-platform goal._

Language chosen: C++

***

### Thinking cross-platform.
> _You have the choice of the main platform during development, but the largest part of the code has to be thinked as compilable on Microsoft Windows and GNU/Linux platforms._
> _Even if it needs some (documented) changes._

Cross-platform Libraries chosen: [SimpleSockets](http://sockets.carrierlabs.com/), [OpenNI](http://openni.org/)

***

### Easy setup.
> _Any configuration shall be editable through a text file (.INI or .XML is good), as ID of the 3D sensor, or Socket Port._
> _A command-line assistant would be appreciable._