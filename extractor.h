#ifndef EXTRACTOR_H
#define EXTRACTOR_H
#include "fileproject.h"
#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include<QDirIterator>
#include <QLabel>
#include <vector>
using std::vector;
QRegExp generateSuffixRegExp(QStringList suffixes);
/*
 * FUNC:generate the regular expression for detecting the suffixes provided by input
 * INPUT:
 * (QStringList)the list of suffixes
 * OUTPUT:the desired expression
 * NOTE: the list of suffixes does not contains the first dot!
 */
QDir getProjectDir();
/*
 * FUNC:ask the user for a list of projects
 * INPUT: none,will ask user
 * OUTPUT:the list of selected directories
 */
QFileInfoList& recurFind(QFileInfoList& result,const QRegExp& pattern,QDir currentDir,
                          bool RETOnFound,QLabel* indicator = NULL);
/*
 * FUNC:search for files whose suffix matches the provided pattern
 * INPUT:
 * (QFileInfoList&)the list for storing the target's absolute path with fileName
 * (QRegExp&)the expression for matching the target
 * (QDir)the place to start looking
 * (bool)should stop searching the subDirectories when there is on found
 * (QLabel*)the label to dump searching progress to
 * OUTPUT:the desired list of targets
 */

void recurDel(QDir inDir);
/*
 * FUNC: Qt4 exclusive,for recursively remove the dir which is already contained in Qt5
 * INPUT:(QDir)the dir to remove
 * OUTPUT:none
*/

QFileInfoList extractProject(const fileProject &inProject, QDir tempDir);
/*
 * FUNC:extract the object to a specific place,and arrange it (temp as default),
 * INPUT:
 * (fileProject&)the project to extract
 * (QDir)the temp dir to use
 * OUTPUT:The list of failed files
 */

#endif // EXTRACTOR_H
