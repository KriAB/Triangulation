#ifndef LASLOADER_H
#define LASLOADER_H

/** LAS file loader
 * Reads an uncompressed LAS file and
 * extracts the points.
 * @authors andesyv, KriAB, Gammelsaeterg, Lillebenn
 */

#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <array>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace gsl
{
constexpr bool VERBOSE = false;
unsigned short getCurrentYear()
{
    auto t = std::time(nullptr);
    return std::localtime(&t)->tm_year + 1900;
}

// TODO update so both 1.2 and 1.4 will work
// - Add 1.3 and 1.4 formats
// NB: For 32 bit compilator
// 64 bit must use other types that are equal in bytesize.
class LASLoader
{
public:
    /** Standard type sizes for 32 bit compiler:
     * char = 1
     * short = 2
     * int = 4
     * long = 4
     * long long = 8
     * float = 4
     * double = 8
     */

    struct HEADERDATATYPES
    {
        char filesignature[4]; //Sier filtypen og må være LASF for LAS
        unsigned short fileSourceID;
        unsigned short globalEncoding;
        unsigned long projectIDGuidData1; // Optional
        unsigned short projectIDGuidData2; // Optional
        unsigned short projectIDGuidData3; // Optional
        unsigned char projectIDGuidData4[8]; // Optional
        unsigned char versionMajor;
        unsigned char versionMinor;
        char systemIdentifier[32];
        char generatingSoftware[32];
        // 90 bytes thus far
        unsigned short fileCreationDayOfYear; // Optional
        unsigned short fileCreationYear; // Optional
        // 4 more optional bytes here
        unsigned short headerSize;
        unsigned long byteOffsetToPointData;
        unsigned long numberOfVarableLengthRecords;
        unsigned char pointDataRecordFormat;
        unsigned short pointDataRecordLength;
        unsigned long legacyNumberOfPointRecords;
        unsigned long legacyNumberOfPointsByReturns[5];
        // 37 more bytes
        double xScaleFactor;
        double yScaleFactor;
        double zScaleFactor;
        double xbyteOffset;
        double ybyteOffset;
        double zbyteOffset;
        double maxUnformattedX;
        double minUnformattedX;
        double maxUnformattedY;
        double minUnformattedY;
        double maxUnformattedZ;
        double minUnformattedZ;
        // 96 more bytes
        // For 1.3 format
        unsigned long long startOfWaveformDataPacketRecord;
        // 8 bytes
        // For 1.4 format
        unsigned long long startOfExtendVariableLength;
        unsigned long numberOfExtendedVariableLengthRecords;
        unsigned long long numberOfPointRecords;
        unsigned long long numberOfPointsByReturn[15];
        // 140 more bytes
    } header;

    struct VARIABLELENGTHHEADERDATA
    {
        unsigned short reserved;
        char UserID[16];
        unsigned short RecordID;
        unsigned short RecordLengthAfterHeader;
        char Description[32];
    };

    class PointDataRecordData
    {
    public:
        PointDataRecordData(unsigned char format) : mFormat{format} {}

        long unformattedX;
        long unformattedY;
        long unformattedZ;
        unsigned short intensity; //Not required
        unsigned char ReturnNumber : 3;
        unsigned char NumberOfReturns : 3;
        unsigned char ScanDirectionFlag : 1;
        unsigned char EdgeOfFlightLine : 1;
        unsigned char classification;
        char scanAngleRank; //(-90 to + 90) - Left side
        unsigned char UserData; //Not required
        unsigned short pointSourceID;

        double& GPSTime() { return _GPSTime; }
        unsigned short& Red() { return (mFormat == 2) ? *((unsigned short*)(&_GPSTime)) : _Red; }
        unsigned short& Green() { return (mFormat == 2) ? *((unsigned short*)(&_GPSTime + sizeof(short))) : _Green; }
        unsigned short& Blue() { return (mFormat == 2) ? *((unsigned short*)(&_GPSTime + 2 * sizeof(short))) : _Blue; }

        // Other functions
        unsigned char getFormat() const { return mFormat; }
        unsigned short getFormatSize() const
        {
            return formatSize(mFormat);
        }

        static unsigned short formatSize(unsigned char format)
        {
            switch (format)
            {
                case 0:
                return 20;
                case 1:
                return 28;
                case 2:
                return 26;
                case 3:
                return 34;
                default:
                return 0;
            }
        }

        double xNorm() const { return _xNormalized; }
        double yNorm() const { return _yNormalized; }
        double zNorm() const { return _zNormalized; }

        double getX(LASLoader* loader) const
        {
            if (loader == nullptr)
                return 0.0;
            return unformattedX * loader->header.xScaleFactor + loader->header.xbyteOffset;
        }
        double getY(LASLoader* loader) const
        {
            if (loader == nullptr)
                return 0.0;
            return unformattedY * loader->header.yScaleFactor + loader->header.ybyteOffset;
        }
        double getZ(LASLoader* loader) const
        {
            if (loader == nullptr)
                return 0.0;
            return unformattedZ * loader->header.zScaleFactor + loader->header.zbyteOffset;
        }

        void setXNormalized(LASLoader* loader)
        {
            if (loader == nullptr)
                return;

            double normalizedX = unformattedX * loader->header.xScaleFactor + loader->header.xbyteOffset;
                   normalizedX = (normalizedX - loader->header.minUnformattedX)/(loader->header.maxUnformattedX - loader->header.minUnformattedX);
            _xNormalized = normalizedX;
        }
        void setYNormalized(LASLoader* loader)
        {
            if (loader == nullptr)
                return;
             double normalizedY = unformattedY * loader->header.yScaleFactor + loader->header.ybyteOffset;
              normalizedY = (normalizedY - loader->header.minUnformattedY)/(loader->header.maxUnformattedY - loader->header.minUnformattedY);
            _yNormalized = normalizedY;
        }
        //Normalized: point - min / max - min
        void setZNormalized(LASLoader* loader)
        {
            if (loader == nullptr)
                return;
           double normalizedZ = unformattedZ * loader->header.zScaleFactor + loader->header.zbyteOffset;
           normalizedZ = (normalizedZ - loader->header.minUnformattedZ)/(loader->header.maxUnformattedZ - loader->header.minUnformattedZ);
           _zNormalized = normalizedZ;
        }

    private:
        // Depending on format
        double _GPSTime;
        unsigned short _Red;
        unsigned short _Green;
        unsigned short _Blue;
        double _xNormalized;
        double _yNormalized;
        double _zNormalized;

        unsigned char mFormat;

    public:
        double x{};
        double y{};
        double z{};
    };

    enum GLOBALENCODINGFLAG
    {
        GPSTimeType = 1,
        WaveformDataPacketsInternal = 2,
        WaveformDataPacketsExternal = 4,
        ReturnNumbersHaveBeenSyntheticallyGenerated = 8,
        WKT = 16
    };

    // Iterator class for point
    class PointIterator
    {
    private:
        unsigned int pointIndex{0};
        LASLoader& loaderRef;
        PointDataRecordData data;
        unsigned int pointAmount;

    public:
        PointIterator(LASLoader& loader, unsigned char format, unsigned int startIndex)
            : pointIndex{startIndex}, loaderRef{loader}, data{format}
        {
            pointAmount = loaderRef.pointCount();

            if (pointIndex > pointAmount)
                pointIndex = pointAmount;

            if (pointIndex < pointAmount)
            {
                // Set stream to point data position offset
                loaderRef.fstrm.seekg(loaderRef.header.byteOffsetToPointData + pointIndex * data.getFormatSize(), loaderRef.fstrm.beg); // byteOffset from beginning

                loaderRef.fstrm.read((char*) &data, data.getFormatSize());
                data.x = data.getX(&loaderRef);
                data.y = data.getY(&loaderRef);
                data.z = data.getZ(&loaderRef);

                data.setXNormalized(&loaderRef);
                data.setYNormalized(&loaderRef);
                data.setZNormalized(&loaderRef);

            }
        }

        PointDataRecordData& operator* ()
        {
            return data;
        }

        PointDataRecordData* operator-> ()
        {
            return &data;
        }

        PointIterator& operator++ ()
        {
            if (loaderRef.fstrm.tellg() != loaderRef.header.byteOffsetToPointData + (pointIndex + 1) * data.getFormatSize())
            {
                loaderRef.fstrm.seekg(loaderRef.header.byteOffsetToPointData + (pointIndex + 1) * data.getFormatSize(), loaderRef.fstrm.beg); // byteOffset from beginning
            }

            loaderRef.fstrm.read((char*) &data, data.getFormatSize());
            ++pointIndex;

            data.x = data.getX(&loaderRef);
            data.y = data.getY(&loaderRef);
            data.z = data.getZ(&loaderRef);

            data.setXNormalized(&loaderRef);
            data.setYNormalized(&loaderRef);
            data.setZNormalized(&loaderRef);

            return *this;
        }

        bool operator!=(const PointIterator& other)
        {
            return pointIndex != other.pointIndex || data.getFormat() != other.data.getFormat();
        }

        PointIterator operator+ (unsigned int offset) const
        {
            return PointIterator{loaderRef, data.getFormat(), (pointIndex + offset < pointAmount) ? pointIndex + offset : pointAmount};
        }

        PointIterator operator- (unsigned int offset) const
        {
            return PointIterator{loaderRef, data.getFormat(), (pointIndex < offset) ? 0 : pointIndex - offset};
        }
    };

    PointIterator begin()
    {
        if (!fileOpened)
            return end();
        return PointIterator{*this, header.pointDataRecordFormat, 0};
    }
    PointIterator end()
    {
        return PointIterator{*this, (fileOpened) ? header.pointDataRecordFormat : std::numeric_limits<unsigned char>::max(), pointCount()};
    }

// Extra loader data
private:
    bool usingCreationDay = false;
    bool usingCreationYear = false;
    bool fileOpened = false;
    unsigned int currentPointSize{0};

    std::ifstream fstrm{};

public:
    LASLoader()
    {

    }

    LASLoader(const std::string& file)
    {
        open(file);
    }

    bool open(const std::string& file)
    {
        if (fstrm.is_open())
        {
            fstrm.close();
        }

        // Check if filename has a filetype
        auto pos = file.find_last_of('.');
        if (pos == file.npos)
        {
            std::cout << "ERROR: filepath must specify a filetype. (path/file.type)" << std::endl;
            fileOpened = false;
            return false;

        }

        // Check filetype
        auto fileType = file.substr(pos + 1);
        std::string uppercase{};
        std::transform(fileType.begin(), fileType.end(), std::back_inserter(uppercase), [](const char& c){ return std::toupper(c); });
        if (uppercase != "LAS")
        {
            std::cout << "ERROR: loader can only read Lidar LAS files." << std::endl;
            fileOpened = false;
            return false;
        }

        // Open file
        fstrm.open(file.c_str(), std::ifstream::in | std::ifstream::binary);
        if (fstrm.is_open())
        {
            std::cout << "LAS file opened: " << file << std::endl;
            // Read in first 90 bytes of header
            fstrm.read((char *) &header, 90);

            // Attempt to read in creationday and creationyear
            int readerCheckpoint = fstrm.tellg();
            unsigned short creationDayYear[2]{1000, 3000};
            fstrm.read((char*) creationDayYear, 2 * sizeof(unsigned short));
            if (creationDayYear[0] < 367)
            {
                if (creationDayYear[0] != 0)
                {
                    header.fileCreationDayOfYear = creationDayYear[0];
                    usingCreationDay = true;
                }
                else
                {
                    usingCreationDay = false;
                }

                if (creationDayYear[1] > 2000 && creationDayYear[1] <= getCurrentYear())
                {
                    header.fileCreationYear = creationDayYear[1];
                    usingCreationYear = true;
                }
                else
                {
                    usingCreationYear = false;
                }
            }
            else
            {
                fstrm.seekg(readerCheckpoint, fstrm.beg);
                usingCreationDay = false;
                usingCreationYear = false;
            }

            // Read 37 more byte
            fstrm.read((char*) &header.headerSize, 37);

            // And now 96 more bytes
            fstrm.read((char*) &header.xScaleFactor, 96);

            // For newer types:
            if (header.versionMajor == 1 && header.versionMinor >= 3)
            {
                fstrm.read((char*) &header.startOfWaveformDataPacketRecord, 8);
            }

            if (header.versionMajor == 1 && header.versionMinor == 4)
            {
                fstrm.read((char*) &header.startOfExtendVariableLength, 140);
            }

            // Done reading header.

            if (VERBOSE)
            {
                std::cout << "System Identifier: " << header.systemIdentifier << ", Generating Software: "<< header.generatingSoftware <<std::endl;
                std::cout << "LAS version: " << static_cast<int>(header.versionMajor) << "." << static_cast<int>(header.versionMinor) << std::endl;
                // std::cout << "variableLengthRecords:  " << loader.header.numberOfVarableLengthRecords << std::endl;
                if (usingCreationDay)
                    std::cout << "Creation day of year: " << header.fileCreationDayOfYear << std::endl;
                if (usingCreationYear)
                    std::cout << "Creation year: " << header.fileCreationYear << std::endl;
                std::cout << "Header size: " << header.headerSize << std::endl;
            }

            if (header.pointDataRecordFormat > 3)
            {
                std::cout << "ERROR: PointDataFormat not supported (yet)!" << std::endl;
                fstrm.close();
                fileOpened = false;
                return false;
            }

            currentPointSize = 0;
            fileOpened = true;
        }
        else
        {
            std::cout << "ERROR: Could not open file for reading: " << file << std::endl;
            fileOpened = false;
        }

        return fileOpened;
    }

    void close()
    {
        fstrm.close();
        fileOpened = false;
    }

    unsigned int pointCount() const
    {
        if (!fileOpened)
            return 0;

        return header.pointDataRecordLength / PointDataRecordData::formatSize(header.pointDataRecordFormat);
    }

    static double length(double &x, double &y, double &z)
    {
        return std::sqrt(std::pow(x, 2.f) + std::pow(y, 2.f) + std::pow(z, 2.f));
    }
    //The points are to big for use in OpenGL, need to normalize
    static std::array<double,3> normalizePoints (double &x, double &y, double &z)
    {
            double l = length(x,y,z);

            if (l > 0)
            {
                x = x / l;
                y = y / l;
                z = z / l;
            }

            return std::array<double,3>{x,y,z};
    }
    // File with full path
    // Stack overflow!
    static std::vector<PointDataRecordData> readLAS(const std::string& file)
    {
        LASLoader loader{};
        std::vector <PointDataRecordData> points;

        if (loader.open(file))
        {
            for (auto it = loader.begin(); it != loader.end(); ++it)
            {
                points.push_back(*it);
            }
        }

        return points;
    }

    static std::vector<PointDataRecordData> readLASNormalized(const std::string& file)
    {
        LASLoader loader{};
        std::vector <PointDataRecordData> points;

        if (loader.open(file))
        {
            for (auto it = loader.begin(); it != loader.end(); ++it)
            {
                normalizePoints(it->x,it->y,it->z);
                points.push_back(*it);
            }
        }

        return points;
    }

    ~LASLoader()
    {
        close();
    }

};

}

#endif // LASLOADER_H
