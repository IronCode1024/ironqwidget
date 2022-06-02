#ifndef IRECENTPROJECTS_H
#define IRECENTPROJECTS_H

#include "RecentProjectsDll_global.h"
/** 最近使用的项目接口，可将 RecentProjects 继承此接口，对外暴露接口封装在此
 * @brief The IRecentProjects class
 */
class RECENTPROJECTSDLL_EXPORT IRecentProjects
{
public:
    IRecentProjects();

    QString RecentProjectFile();


};

#endif // IRECENTPROJECTS_H
