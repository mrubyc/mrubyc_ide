/*
  mruby/c IDE

  Copyright (C) 2016- Kyushu Institute of Technology.
  Copyright (C) 2016- Shimane IT Open-Innovation Center.
  Copyright (C) 2016- Software Research Associates, Inc.

   All rights reserved. See LICENSE File.
*/

#include "mrcprojectfilecontrol.h"
#include "mrcfile.h"
#include "mrcproject.h"
#include "mrctextdocument.h"
#include "mrubyc_ide.h"

#include <QList>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QtXml>
#include <algorithm>

MrcProjectFileControl::MrcProjectFileControl(QString projectPath, QString projectName, QObject *parent)
    : QObject(parent)
{
    m_projectPath = projectPath;
    m_projectName = projectName;
    m_mrcpFilePath = MrcProject::mrcpFilePath(projectPath, projectName);
}

MrcProjectFileControl::~MrcProjectFileControl()
{
}
bool MrcProjectFileControl::append(MrcFile *mrcFile)
{
    QFile xmlFile(m_mrcpFilePath);
    if (!xmlFile.open(QFile::ReadOnly)) {
        return false;
    }
    QDomDocument document;

    bool result = readXmlDocument(&xmlFile, &document);
    xmlFile.close();
    if (!result) return false;

    QString absolutefilePath = mrcFile->path();
    bool ok;
    QDomNode targetNode = find(&document, absolutefilePath, &ok);
    // If it is something issue or node exists, return false.
    if (!ok || !targetNode.isNull()) return false;

    // Append MrcFile as a dom node into files dom node.
    if (!appendDomNode(&document, mrcFile)) return false;

    // Write the document to the xml file.
    if (!xmlFile.open(QFile::WriteOnly|QFile::Truncate)) {
        return false;
    }
    writeXmlDocument(&xmlFile, &document);
    xmlFile.close();
    return true;
}

bool MrcProjectFileControl::remove(const QString &filePath)
{
    QFile xmlFile(m_mrcpFilePath);
    if (!xmlFile.open(QFile::ReadOnly)) {
        return false;
    }
    QDomDocument document;

    bool result = readXmlDocument(&xmlFile, &document);
    xmlFile.close();
    if (!result) return false;

    bool ok;
    QDomNode targetNode = find(&document, filePath, &ok);
    // If it is something issue or node does not exist, return false.
    if (!ok || targetNode.isNull()) return false;

    // Remove file node from files node.
    QDomNode parentNode = targetNode.parentNode();
    parentNode.removeChild(targetNode);

    // Write the document to the xml file.
    if (!xmlFile.open(QFile::WriteOnly|QFile::Truncate)) {
        return false;
    }
    writeXmlDocument(&xmlFile, &document);
    xmlFile.close();
    return true;
}

bool MrcProjectFileControl::rename(const QString &fromFilePath, const QString &toFilePath)
{
    QFile xmlFile(m_mrcpFilePath);
    if (!xmlFile.open(QFile::ReadOnly)) {
        return false;
    }
    QDomDocument document;

    bool result = readXmlDocument(&xmlFile, &document);
    xmlFile.close();
    if (!result) return false;

    bool ok;
    QDomNode fromTargetNode = find(&document, fromFilePath, &ok);
    // If it is something issue or node does not exist, return false.
    if (!ok || fromTargetNode.isNull()) return false;

    QDomNode toTargetNode = find(&document, toFilePath, &ok);
    // If it is something issue or node does not exist, return false.
    if (!ok || !toTargetNode.isNull()) return false;

    // Rename name attribute.
    QDomElement fileElem = fromTargetNode.toElement();
    QString toRelativePath = MrcProject::relativePath(m_projectPath, toFilePath);
    fileElem.setAttribute("name", toRelativePath);
    // Update type
    QDomElement typeElem = fromTargetNode.firstChildElement("type");
    MrcAbstractFile::MrcFileType fileType = MrcAbstractFile::mrcFileType(toFilePath);
    QDomText typeText = document.createTextNode(QString::number(fileType));
    if (typeElem.firstChild().isNull()) {
        typeElem.appendChild(typeText);
    } else {
        QDomNode oldMarkNode = typeElem.firstChild();
        typeElem.replaceChild(typeText, oldMarkNode);
    }


    // Write the document to the xml file.
    if (!xmlFile.open(QFile::WriteOnly|QFile::Truncate)) {
        return false;
    }
    writeXmlDocument(&xmlFile, &document);
    xmlFile.close();
    return true;
}

bool MrcProjectFileControl::update(MrcFile *mrcFile)
{
    QFile xmlFile(m_mrcpFilePath);
    if (!xmlFile.open(QFile::ReadOnly)) {
        return false;
    }
    QDomDocument document;

    bool result = readXmlDocument(&xmlFile, &document);
    xmlFile.close();
    if (!result) return false;

    QString absolutefilePath = mrcFile->path();
    bool ok;
    QDomNode targetNode = find(&document, absolutefilePath, &ok);
    // If it is something issue or node does not exist, return false.
    if (!ok || targetNode.isNull()) return false;

    // Update editing
    QDomElement editingElem = targetNode.firstChildElement("editing");
    QDomText editingText = document.createTextNode(BOOL2STRING(mrcFile->isEditing()));
    if (editingElem.firstChild().isNull()) {
        editingElem.appendChild(editingText);
    } else {
        QDomNode oldEditingNode = editingElem.firstChild();
        editingElem.replaceChild(editingText, oldEditingNode);
    }
    // Update mark
    QDomElement markElem = targetNode.firstChildElement("mark");
    QDomText markText = document.createTextNode(toMarkString(mrcFile));
    if (markElem.firstChild().isNull()) {
        markElem.appendChild(markText);
    } else {
        QDomNode oldMarkNode = markElem.firstChild();
        markElem.replaceChild(markText, oldMarkNode);
    }

    // Write the document to the xml file.
    if (!xmlFile.open(QFile::WriteOnly|QFile::Truncate)) {
        return false;
    }
    writeXmlDocument(&xmlFile, &document);
    xmlFile.close();
    return true;
}

void MrcProjectFileControl::save(MrcProject *mrcProject)
{
    // STEP1: Create a dom document to write
    QString versionNum = "1.0"; // \todo FIXME: where should the version be defined?
    QDomDocument document;

    // create element.
    QDomElement rootElem = document.createElement("mrcp");
    rootElem.setAttribute("name", m_projectName);

    QDomElement versionElem = document.createElement("version");
    QDomElement timestampElem = document.createElement("timestamp");
    QDomElement filesElem = document.createElement("files");
    // set value.
    QDomText verText = document.createTextNode(versionNum);
    versionElem.appendChild(verText);
    QDateTime date = QDateTime::currentDateTime();
    QDomText timestampText = document.createTextNode(date.toString(Qt::ISODate));
    timestampElem.appendChild(timestampText);

    // construct node.
    document.appendChild(rootElem);

    rootElem.appendChild(versionElem);
    rootElem.appendChild(timestampElem);
    rootElem.appendChild(filesElem);

    QList<MrcFile*> projectFileList = mrcProject->projectMrcFileList();
    foreach (MrcFile* mrcFile, projectFileList) {
        QString absolutefilePath = mrcFile->path();
        QString filePath = MrcProject::relativePath(m_projectPath, absolutefilePath);

        // create element.
        QDomElement fileElem = document.createElement("file");
        fileElem.setAttribute("name", filePath);
        QDomElement typeElem = document.createElement("type");
        QDomElement editingElem = document.createElement("editing");
        QDomElement markElem = document.createElement("mark");
        // set value
        QDomText typeText = document.createTextNode(QString::number(mrcFile->fileType()));
        QDomText editiongText = document.createTextNode(BOOL2STRING(mrcFile->isEditing()));
        if (mrcFile->isEditing() && mrcFile->markList().size() > 0) {
            QDomText markText = document.createTextNode(toMarkString(mrcFile));
            markElem.appendChild(markText);
        }
        typeElem.appendChild(typeText);
        editingElem.appendChild(editiongText);

        //construct a file node.
        fileElem.appendChild(typeElem);
        fileElem.appendChild(editingElem);
        fileElem.appendChild(markElem);

        // add a file node into fileList.
        filesElem.appendChild(fileElem);
    }

    // STEP2: Write the xml fie from a dom document.
    QFile xmlFile(m_mrcpFilePath);
    if (!xmlFile.open(QFile::WriteOnly|QFile::Truncate)) {
        return;
    }
    // write a node document to the xml file.
    writeXmlDocument(&xmlFile, &document);
    xmlFile.close();
}


bool MrcProjectFileControl::load(MrcProject *mrcProject)
{
    QFile xmlFile(m_mrcpFilePath);
    if (!xmlFile.open(QFile::ReadOnly)) {
        return false;
    }

    QDomDocument document;

    bool result = readXmlDocument(&xmlFile, &document, mrcProject);

    xmlFile.close();
    return result;
}


MrcFile *MrcProjectFileControl::addFile(const QString &name)
{
    // check that a file exists.
    QFileInfo fileInfo(name);
    if (!fileInfo.isFile()
            || !fileInfo.isReadable()) {
        return Q_NULLPTR; // FIMXE: if issue, return null???
    }
    MrcFile *mrcFile = new MrcFile(name);
    return mrcFile;
}

QString MrcProjectFileControl::toMarkString(MrcFile *mrcFile)
{
    QString markNumbers;

    // If a file is not editing, do not save mark.
    if (!mrcFile->isEditing()) return markNumbers;

    // for Mark
    QList<int> markList = mrcFile->markList();
    QList<int>::const_iterator itr = markList.begin();
    while (itr != markList.cend()) {
        int pos = *itr;
        if (markNumbers.isEmpty()) {
            markNumbers.append(QString::number(pos));
        } else {
            markNumbers.append(",").append(QString::number(pos));
        }
        ++itr;
    }
    return markNumbers;
}

QDomNode MrcProjectFileControl::find(QDomDocument *document, const QString &filePath, bool *ok)
{
    QString relativePath = MrcProject::relativePath(m_projectPath, filePath);
    QDomNode foundNode;

    // get parent node.
    QDomNodeList filesNodeList = document->elementsByTagName("files");
    if (filesNodeList.isEmpty()) {
        if (ok) *ok = false;
        return foundNode;
    }
    QDomNode parentNode = filesNodeList.at(0);
    QDomNodeList fileNodeList = parentNode.childNodes();
    for (int i=0; i<fileNodeList.size(); i++) {
        QDomNode fileNode = fileNodeList.at(i);
        if (!fileNode.isElement()) continue;

        QDomElement fileElem = fileNode.toElement();
        QString nameAttribute = fileElem.attribute("name");
        if (nameAttribute == relativePath) {
            foundNode = fileNode;
            break;
        }
    }
    if (ok) *ok = true;
    return foundNode;
}

bool MrcProjectFileControl::appendDomNode(QDomDocument *document, MrcFile *mrcFile)
{
    // Get parent node.
    QDomNodeList nodeList = document->elementsByTagName("files");
    if (nodeList.isEmpty()) return false;

    QDomNode parentNode = nodeList.at(0); // It takes care of a first node.

    QString absoluteFilePath = mrcFile->path();
    QString relativePath = MrcProject::relativePath(m_projectPath, absoluteFilePath);
    // create element.
    QDomElement fileElem = document->createElement("file");
    fileElem.setAttribute("name", relativePath);
    QDomElement typeElem = document->createElement("type");
    QDomElement editingElem = document->createElement("editing");
    QDomElement markElem = document->createElement("mark");
    // set value
    QDomText typeText = document->createTextNode(QString::number(mrcFile->fileType()));
    QDomText editiongText = document->createTextNode(BOOL2STRING(mrcFile->isEditing()));
    if (mrcFile->isEditing() && mrcFile->markList().size() > 0) {
        QDomText markText = document->createTextNode(toMarkString(mrcFile));
        markElem.appendChild(markText);
    }
    typeElem.appendChild(typeText);
    editingElem.appendChild(editiongText);
    //construct a file node.
    fileElem.appendChild(typeElem);
    fileElem.appendChild(editingElem);
    fileElem.appendChild(markElem);

    // add a file node into fileList.
    parentNode.appendChild(fileElem);
    return true;
}

void MrcProjectFileControl::writeXmlDocument(QIODevice *device, QDomDocument *document)
{
    QDomNode processingNode = document->firstChild();
    if (!processingNode.isProcessingInstruction()) {
        QDomNode xmlNode = document->createProcessingInstruction("xml", "version=\'1.0\' encoding=\'UTF-8\'");
        document->insertBefore(xmlNode, document->firstChild());
    }

    QTextStream out(device);
    document->save(out, MRUBYC_IDE::XML_INDENT);
}

bool MrcProjectFileControl::readXmlDocument(QIODevice *device, QDomDocument *document, MrcProject *mrcProject)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    const auto parseResult = document->setContent(device);
    if (!parseResult) {
        errorStr = parseResult.errorMessage;
        errorLine = parseResult.errorLine;
        errorColumn = parseResult.errorColumn;

        qCritical() << tr("Parse error at line %1, column %2:\n%3")
                        .arg(errorLine)
                        .arg(errorColumn)
                        .arg(errorStr);
        return false;
    }
    if (mrcProject == Q_NULLPTR) {
        return true;
    }

    QDomElement root = document->firstChildElement("mrcp");
    if (root.isNull()) {
        qDebug() << tr("mrcp tag does not exist.");
        return false;
    }

    QString name = root.attribute("name");
    if (name.isNull()) {
        qDebug() << tr("name attribute is not found.");
        return false;
    }
//    mrcProject->name = name;

    QDomElement versionElem = root.firstChildElement("version");
    QString version = versionElem.text();
    mrcProject->setVersion(version);

    QDomElement timestampElem = root.firstChildElement("timestamp");
    QString timestamp = timestampElem.text();
    mrcProject->setTimeStamp(timestamp);

    QDomElement filesElem = root.firstChildElement("files");
    QDomNodeList fileList = filesElem.childNodes();
    for (int j=0; j<fileList.size(); j++) {
        QDomNode node = fileList.at(j);

        QDomElement fileElem = node.toElement();
        QString relativeFilePath = fileElem.attribute("name");
        QString absoluteFilePath = MrcProject::absolutePath(m_projectPath, relativeFilePath);

        MrcFile *mrcFile = addFile(absoluteFilePath);
        if (mrcFile == Q_NULLPTR) continue;

        mrcProject->addMrcFile(absoluteFilePath, mrcFile);

        QDomElement typeElem = node.firstChildElement("type");
        int type = typeElem.text().toInt();
        MrcAbstractFile::MrcFileType mrcType = static_cast<MrcAbstractFile::MrcFileType>(type);
        mrcFile->setFileType(mrcType);

        QDomElement editingElem = node.firstChildElement("editing");
        bool editing = editingElem.text()=="true"?true:false;
        mrcFile->setEditing(editing);

        QDomElement markElem = node.firstChildElement("mark");
        QString marks = markElem.text();
        // for Mark
        QList<int> markList;
        // It should be populated the mark whenenver editing is true.
        if (editing && !marks.isEmpty()) {
            QStringList markNumberList = marks.split(',');
            for (int j=0; j<markNumberList.size(); j++) {
                QString markNumber = markNumberList.at(j);
                if (markNumber.isNull()) continue;
                bool ok = true;
                int markNum = markNumber.toInt(&ok);
                if (!ok) continue;
                // Do not add same number into a list.
                if (markList.contains(markNum)) continue;
                markList.append(markNum);
            }
            if (!markList.isEmpty() && markList.size() > 1) {
                std::sort(markList.begin(), markList.end());
            }
            mrcFile->setMarkList(markList);
        }

    }
    return true;
}
