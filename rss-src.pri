RSSGUI_DIR = ../rss-gui

SOURCES += $$RSSGUI_DIR/mainwindow.cpp \
    $$RSSGUI_DIR/rss.cpp \
    $$RSSGUI_DIR/news.cpp \
    $$RSSGUI_DIR/rssreader.cpp \
    $$RSSGUI_DIR/rsshandler.cpp \
    $$RSSGUI_DIR/newsmodel.cpp \
    $$RSSGUI_DIR/decoratednewsmodel.cpp \
    $$RSSGUI_DIR/util.cpp \
    $$RSSGUI_DIR/imagecache.cpp

HEADERS  += $$RSSGUI_DIR/mainwindow.h \
    $$RSSGUI_DIR/rss.h \
    $$RSSGUI_DIR/news.h \
    $$RSSGUI_DIR/rssreader.h \
    $$RSSGUI_DIR/rsshandler.h \
    $$RSSGUI_DIR/newsmodel.h \
    $$RSSGUI_DIR/decoratednewsmodel.h \
    $$RSSGUI_DIR/util.h \
    $$RSSGUI_DIR/imagecache.h

FORMS    += $$RSSGUI_DIR/mainwindow.ui
