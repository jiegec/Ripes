TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src app test runner
test.depends = src
runner.depends = src
app.depends = src
