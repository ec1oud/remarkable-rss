TEMPLATE = subdirs

SUBDIRS += \
    rss-test rss-gui

rss-test.depends = rss-gui
