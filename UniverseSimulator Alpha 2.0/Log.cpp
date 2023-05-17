#include <QDateTime>
#include <QFile>
#include <QDir>
#include "Log.h"


//此处代码参考自https://blog.csdn.net/m0_49456900/article/details/124928129


QString createMultipleFolders(const QString path)
{
    QDir dir(path);
    if (dir.exists(path)) {
        return path;
    }

    QString parentDir = createMultipleFolders(path.mid(0, path.lastIndexOf('\\')));//截取根目录
    QString dirName = path.mid(path.lastIndexOf('\\') + 1);//截取父目录

    QDir parentPath(parentDir);
    if (!dirName.isEmpty())
    {
        parentPath.mkpath(dirName);
    }

    return parentDir + "\\" + dirName;
}


QString createFile(const QString path,const QString suffix)
{
    QString createTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");//文件名不能用“:”命名
    QString fileName = path + "\\" + createTime + suffix;
    QFile file(fileName);

    if(file.exists())
    {
        return fileName;
    }
    else
    {
        // 如果没有此文件，就创建
        file.open(QFile::WriteOnly|QFile::Text|QIODevice::Append);
    }

    return fileName;
}
