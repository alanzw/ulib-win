@echo off

rm -vf *.ncb *.opt

rm -rvf examples/base/Debug
rm -vf examples/base/*.plg

rm -rvf examples/window/Debug
rm -vf examples/window/*.plg

rm -rvf examples/wtlbase/Debug
rm -vf examples/wtlbase/*.plg

rm -rvf examples/wtlclock/Debug
rm -vf examples/wtlclock/*.plg

@pause
