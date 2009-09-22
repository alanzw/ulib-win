' A VBscript example of using IExample2.dll
set myObj = CreateObject("IExample2.object")
myObj.Buffer = "Hello world"
MsgBox myObj.Buffer, 0, "GetString return"
