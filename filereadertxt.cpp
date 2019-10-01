#include "filereadertxt.h"
#include <fstream>
#include <ostream>

FileReaderTXT::FileReaderTXT()
{

}

FileReaderTXT::FileReaderTXT(std::string filename)
{
    std::ifstream inn;
    std::string fileWithPath = "../Triangulation/" + filename;

    inn.open(fileWithPath);

    if (inn.is_open())
    {
        unsigned int n;
        gsl::Vector3D vertex;
        inn >> n;
        points.reserve(n);
        for (unsigned int i = 0; i < n; i++)
        {
            inn >> vertex;
            points.push_back(vertex);

            std::string str;
            std::getline(inn, str);
        }
        inn.close();
        //qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);
    }
    else
    {
        //qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }
}

void FileReaderTXT::PrintCoordinates()
{
    std::cout << points.size() << "\n";
    for (auto point : points)
    {
        std::cout << point;
        std::cout << "\n";
    }
}

std::ostream& operator<< (std::ostream &os, const gsl::Vector3D &v)
{
    os << std::fixed;
    os << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ") ";
    return os;
}

