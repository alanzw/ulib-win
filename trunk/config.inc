KENERL_OBJS =     \
    ulib.o        \


GUI_OBJS =        \
    umsg.o        \
    ucontrol.o    \
    ustatic.o     \
    uimagelist.o  \
    ubitmap.o     \
    ubutton.o     \
    udialog.o     \
    udialogx.o    \
    uedit.o       \
    ugldialog.o   \
    uupdownctrl.o \
    utreeview.o   \
    ulistview.o   \
    ulistbox.o    \
    ucombobox.o   \
    ucomboboxex.o \
    uanimation.o  \
    uprogressbar.o\
    ustatusbar.o  \
    ucustomctrl.o \
    utrackbar.o   \
    umessageonlywindow.o \
    utabctrl.o    \
    ubasewindow.o \
    utoolbar.o    \
    usplashwnd.o  \
    uglwindow.o   \


SYSTEM_OBJS =     \
    uconsole.o    \
    udebug.o      \
    ufileman.o    \
    udllman.o     \
    uobject.o     \
    ugdi.o        \
    uevent.o      \
    ucritical.o   \
    usemaphore.o  \
    umutex.o      \
    utoolhelp.o   \
    ufilemapping.o\
    umailslot.o   \
    upipe.o       \
    uthunk32.o    \
    uimagehlp.o   \
    usync.o       \
    ufiber.o      \


GRAPHIC_OBJS =    \
    uglut.o       \
    udibapi.o     \
    urecttracker.o\
    uvisualstyle.o\
    upicture.o    \
    ugdiplus.o    \


CONFIG_OBJS  =    \
    uini.o        \
    uconf.o       \
    ucom.o        \

CRYPTO_OBJS =     \
    md5.o         \
    crc32.o       \
    base64.o      \


ifeq ($(PLATFORM), win32)
    DB_OBJS =         \
        uodbc.o       \

else
    DB_OBJS =
endif


OBJS = $(KENERL_OBJS)  $(GUI_OBJS) $(SYSTEM_OBJS) $(GRAPHIC_OBJS) $(CONFIG_OBJS) $(CRYPTO_OBJS) \
       $(DB_OBJS)    \
       ulink.o       \
       usys.o        \
       umenu.o       \
       uplugin.o     \
       uscrollbar.o  \
       urt.o         \
       utime.o       \
       utimer.o      \
       ucommondialog.o \
       uheader.o     \
       ukeybd.o      \
       uprocess.o    \
       uprocessman.o \
       uthread.o     \
       untservice.o  \
       udraglistbox.o\
       uclipboard.o  \
       uregkey.o     \
       urichedit.o   \
       ufiledb.o     \
       uipaddress.o  \
       urebar.o      \
       udatetimepick.o \
       uiphlp.o      \
       uwindowx.o    \
       ufont.o       \
       upanel.o     \
       uhotkey.o     \
       udbman.o      \
       ufile.o       \
       usocket.o     \
       ulayout.o     \
       utooltip.o    \
       uicon.o       \
       ulog.o        \
       uwinapp.o     \
       uregion.o     \
       ucmdline.o    \
       uprinter.o    \
       ucursor.o     \
       ucaret.o      \
       udc.o         \
       ushell.o      \
       umci.o        \
       umixer.o      \
       ubrush.o      \
       upalette.o    \
       uresource.o   \
       uregex.o      \
       upen.o        \
