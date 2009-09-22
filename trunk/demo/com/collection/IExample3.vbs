' A VBscript example of using IExample3.dll
set myObj = CreateObject("IExample3.object")
Set coll = myObj.Ports
count = coll.Count
For i = 0 To count - 1
    MsgBox coll.item(i)
Next


