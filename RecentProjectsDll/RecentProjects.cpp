#include "RecentProjects.h"

RecentProjects::RecentProjects(QWidget *parent)
{
//    createActions();
//    createMenus();
//    (void)statusBar();

//    setWindowFilePath(QString());

}
//单击菜单动作 triggered
void RecentProjects::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action)
    {
        QString filePath = action->text().split("|")[1].simplified();
        QDir tempDir;
        //如果 fileName 路径不存在，则删除这条历史记录
        if (!tempDir.exists(filePath))
        {
            qDebug() << QString::fromLocal8Bit("不存在该路径");
            QMessageBox msgBox;
            msgBox.setText(QString::fromLocal8Bit("文件不存在，是否将其从最近中移除！"));
            msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            msgBox.setButtonText(QMessageBox::Ok, QString::fromLocal8Bit("确定"));
            msgBox.setButtonText(QMessageBox::Cancel, QString::fromLocal8Bit("取消"));
            if(msgBox.exec() == QMessageBox::Ok)
            {
                remove(filePath);
            }
            return;
        }
        //路径存在才发出触发的信号
        emit emit_triggered(filePath);
        //loadFile(action->data().toString());
    }
}
void RecentProjects::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Recent Files"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(fileName)
//                             .arg(file.errorString()));
        qDebug()<<(tr("Recent Files"),
                    tr("Cannot read file %1:\n%2.")
                    .arg(fileName)
                    .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
//    textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
//    statusBar()->showMessage(tr("File loaded"), 2000);
}
void RecentProjects::createActions()
{
    for (int i = 0; i < MaxRecentFiles; ++i) {
        //初始化一个菜单项
        recentFileActs.append(new QAction(this));
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }
}
void RecentProjects::createMenus()
{
//    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(newAct);
//    fileMenu->addAction(openAct);
//    fileMenu->addAction(saveAct);
//    fileMenu->addAction(saveAsAct);
//    separatorAct = fileMenu->addSeparator();
//    for (int i = 0; i < MaxRecentFiles; ++i)
//        fileMenu->addAction(recentFileActs[i]);
//    fileMenu->addSeparator();
//    fileMenu->addAction(exitAct);


    this->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        this->addAction(recentFileActs[i]);

    this->addSeparator();
    updateRecentFileActions();

//    menuBar()->addSeparator();

//    helpMenu = menuBar()->addMenu(tr("&Help"));
//    helpMenu->addAction(aboutAct);
//    helpMenu->addAction(aboutQtAct);
}
void RecentProjects::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
//    setWindowFilePath(curFile);

    //保存到 ini文件中
    QSettings settings(saveRecentFile, QSettings::IniFormat);
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);//先将相同的记录删除
    files.prepend(fileName);//添加新的记录到最前面
    //数量大于允许的最大值，则将最后一个删除（最早的一条数据）
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();

//    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
//        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
//        if (mainWin)
//            mainWin->updateRecentFileActions();
//    }
}
/**！刷新界面显示，将最近的内容添加到菜单中
 * @brief RecentProjects::updateRecentFileActions
 */
void RecentProjects::updateRecentFileActions()
{
    QSettings settings(saveRecentFile, QSettings::IniFormat);
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
//        QDir tempDir;
//        //如果 fileName 路径不存在，则删除这条历史记录
//        if (!tempDir.exists(files[i]))
//        {
//            qDebug() << "不存在该路径" << endl;
//            return;
//        }
        QString text = tr("&%1 | %2").arg(i + 1).arg(files[i]);//(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

//    separatorAct->setVisible(numRecentFiles > 0);
}

//从最近中移除
bool RecentProjects::remove(const QString &fileName)
{
    QSettings settings(saveRecentFile, QSettings::IniFormat);
    QStringList files = settings.value("recentFileList").toStringList();
    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);//先拿到所有最近文件数，然后再删除
    files.removeAll(fileName);//从 saveRecentFile ini 文件中删除
    settings.setValue("recentFileList", files);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = recentFileActs[i]->text().split('|')[1].simplified();
        if(text == fileName)
        {
            recentFileActs[i]->setVisible(false);
            recentFileActs[i]->deleteLater();
            recentFileActs[i] = NULL;
            recentFileActs.removeAt(i);
            return true;
        }
    }
    return false;
//    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
//        recentFileActs[j]->setVisible(false);
}

QString RecentProjects::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
/** 持久化保存最近使用的文件列表
 * @brief RecentProjects::setSaveRecentFile
 * @param fileName
 */
void RecentProjects::setSaveRecentFile(const QString& fileName)
{
    saveRecentFile = fileName;
    createActions();
    createMenus();
}
/** 设置显示最近使用的最大文件数,默认值为 5
 * @brief RecentProjects::setMaxRecentFiles
 * @param fileName
 */
void RecentProjects::setMaxRecentFiles(const int maxRecentFiles)
{
    MaxRecentFiles = maxRecentFiles;
}
