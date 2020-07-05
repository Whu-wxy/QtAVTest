#-------------------------------------------------
#
# Project created by QtCreator 2020-05-13T20:17:30
#
#-------------------------------------------------

QT       += core gui av sensors  #avwidgets
LIBS += -lQtAV #-lQtAVWidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtAVTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    zndkandroiddlg.cpp \
    zndkavwidget.cpp \
    zndksettingdlg.cpp \
    WidgetRenderer.cpp \
    global.cpp

HEADERS += \
    zndkandroiddlg.h \
    zndkavwidget.h \
    zndksettingdlg.h \
    WidgetRenderer.h \
    global.h

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../QtAV/ffmpeg-master-android-clang-lite/lib/armeabi-v7a/libavcodec.so \
#        $$PWD/../../QtAV/ffmpeg-master-android-clang-lite/lib/armeabi-v7a/libavfilter.so \
        $$PWD/../../QtAV/ffmpeg-master-android-clang-lite/lib/armeabi-v7a/libavformat.so \
        $$PWD/../../QtAV/ffmpeg-master-android-clang-lite/lib/armeabi-v7a/libavutil.so \
#        $$PWD/../../QtAV/ffmpeg-master-android-clang-lite/lib/armeabi-v7a/libffmpeg.so \
        $$PWD/../../QtAV/ffmpeg-master-android-clang-lite/lib/armeabi-v7a/libswresample.so \
        $$PWD/../../QtAV/ffmpeg-master-android-clang-lite/lib/armeabi-v7a/libswscale.so
}

