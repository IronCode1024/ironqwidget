#ifndef RECENTPROJECTS_H
#define RECENTPROJECTS_H
#include "RecentProjectsDll_global.h"
#include "IRecentProjects.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

/** 最近使用的工程
 * @brief The RecentProjects class
 * 调用方法：
   最近使用项目 菜单
    RecentProjects* recentProjects = new RecentProjects();
    //绑定选中菜单槽函数，返回文件路径
    connect(recentProjects,&RecentProjects::emit_triggered,this,[=](const QString& str){
        openProject(str);
    });
以下两种方法任选一种
    recentProjects->setSaveRecentFile("RecentProjectsConfig.ini");
    QMenu* m = ui->menu;
   1、作为Action的子菜单
    QAction* a1= new QAction(QString::fromLocal8Bit("最近打开项目"));
    m->addAction(a1);
    a1->setMenu(recentProjects);
    m->addMenu(recentProjects);
    //更新 最近使用的工程菜单
    recentProjects->setCurrentFile("D:\\aaa.txt");

   2、做为Actions 菜单
    m->addSeparator();
    m->addActions(recentProjects->actions());
    m->addSeparator();
    //更新 最近使用的工程菜单
    recentProjects->setCurrentFile("D:\\aaa.txt");
 */
class RECENTPROJECTSDLL_EXPORT RecentProjects : public QMenu //,public IRecentProjects
{
    Q_OBJECT
public:
    RecentProjects(QWidget *parent = nullptr);

    /** 设置当前打开文件到最近打开文件中
     * @brief setCurrentFile
     * @param fileName
     */
    void setCurrentFile(const QString &fileName);
    /** 持久化保存文件
     * @brief setSaveRecentFile
     * @param fileName
     */
    void setSaveRecentFile(const QString& fileName);
    /** 设置显示最近使用的最大文件数
     * @brief RecentProjects::setMaxRecentFiles
     * @param fileName
     */
    void setMaxRecentFiles(const int maxRecentFiles);
signals:
    /** 触发菜单动作
     * @brief emit_triggered
     * @param str
     */
    void emit_triggered(const QString& str);
private slots:
    void openRecentFile();
private:
    void createActions();
    void createMenus();
    void loadFile(const QString &fileName);
    void updateRecentFileActions();
    //从最近中移除
    bool remove(const QString &fileName);
    //截取文件名称，包含后缀
    QString strippedName(const QString &fullFileName);

    //持久化保存文件
    QString saveRecentFile;
    QString curFile;
    //显示最近打开的最大数
    int MaxRecentFiles = 5;

//    enum { MaxRecentFiles = 4 };
//    QAction *recentFileActs[MaxRecentFiles];
    QList<QAction*> recentFileActs;
};

#endif // RECENTROJEPCTSH_
