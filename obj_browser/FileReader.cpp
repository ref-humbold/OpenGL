#include <exception>
#include <stdexcept>

#include "FileReader.hpp"

vec3 normalVec(vec3 vt1, vec3 vt2, vec3 vt3)
{
    vec3 vt12 = vt2 - vt1;
    vec3 vt13 = vt3 - vt1;

    return normalize(cross(vt12, vt13));
}

std::vector<std::string> split(const std::string & str, const std::string & delim)
{
    std::vector<std::string> splitted;
    size_t beginPos = 0;

    while(beginPos != std::string::npos)
    {
        size_t endPos = str.find(delim, beginPos);

        if(endPos != std::string::npos)
        {
            splitted.push_back(str.substr(beginPos, endPos - beginPos));
            beginPos = endPos + delim.size();
        }
        else
        {
            splitted.push_back(str.substr(beginPos));
            beginPos = endPos;
        }
    }

    return splitted;
}

std::vector<GLuint> parse(const std::string & str, const std::string & delim)
{
    std::vector<GLuint> parsed;
    std::vector<std::string> splitted = split(str, delim);

    parsed.resize(splitted.size());

    std::transform(splitted.begin(), splitted.end(), parsed.begin(),
                   [](std::string s) { return s == "" ? 0 : stoul(s); });

    return parsed;
}

void readOBJ(GraphicObject * grobj, const char * filename)
{
    FILE * file = fopen(filename, "r");

    if(file == nullptr)
        throw std::runtime_error("Cannot open OBJ file.");

    std::vector<vec3> vertRead;
    std::vector<vec2> uvRead;
    std::vector<vec3> normRead;
    std::vector<GLuint> vertIndices;
    std::vector<GLuint> uvIndices;
    std::vector<GLuint> normIndices;
    std::vector<vec3> vertices;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;

    while(true)
    {
        char str[1024];
        char * read = fgets(str, 1023, file);

        if(read == nullptr)
            break;

        std::string sstr = std::string(str);
        int lastNL = sstr.find_last_of("\n");
        std::string trim_sstr = sstr.substr(0, lastNL);

        std::vector<std::string> line = split(trim_sstr, " ");

        if(line[0] == "v")
        {
            if(line.size() >= 4)
                vertRead.push_back(vec3(stof(line[1]), stof(line[2]), stof(line[3])));
            else if(line.size() == 3)
                vertRead.push_back(vec3(stof(line[1]), stof(line[2]), 0.0f));
            else
                throw std::runtime_error("Invalid vertex coordinates.");
        }
        else if(line[0] == "vt")
        {
            if(line.size() >= 3)
                uvRead.push_back(vec2(stof(line[1]), stof(line[2])));
            else
                throw std::runtime_error("Invalid texture coordinates.");
        }
        else if(line[0] == "vn")
        {
            if(line.size() >= 4)
                normRead.push_back(vec3(stof(line[1]), stof(line[2]), stof(line[3])));
            else
                throw std::runtime_error("Invalid normal vector coordinates.");
        }
        else if(line[0] == "f")
        {
            std::vector<std::vector<GLuint>> indx(3);

            if(line.size() < 4)
                throw std::runtime_error("Expected three sets of indices for vertices");

            std::transform(line.begin() + 1, line.begin() + 4, indx.begin(),
                           [](const std::string & s) { return parse(s, "/"); });

            vec3 countedNormal = normalVec(vertRead[indx[0][0] - 1], vertRead[indx[1][0] - 1],
                                           vertRead[indx[2][0] - 1]);

            if(std::any_of(indx.begin(), indx.end(), [](const std::vector<GLuint> & vc) {
                   return vc.size() < 3 || vc[2] == 0;
               }))
            {
                normRead.push_back(countedNormal);
            }

            for(auto vc : indx)
            {
                vertIndices.push_back(vc[0] - 1);

                if(vc.size() >= 2)
                {
                    if(vc[1] > 0)
                        uvIndices.push_back(vc[1] - 1);

                    if(vc.size() >= 3)
                    {
                        if(vc[2] > 0)
                            normIndices.push_back(vc[2] - 1);
                        else
                            normIndices.push_back(normRead.size() - 1);
                    }
                }
                else
                    normIndices.push_back(normRead.size() - 1);
            }
        }
    }

    for(auto ix : vertIndices)
        vertices.push_back(vertRead[ix]);

    for(auto ix : uvIndices)
        uvs.push_back(uvRead[ix]);

    for(auto ix : normIndices)
        normals.push_back(normRead[ix]);

    grobj->setVertexBuffer(vertices);
    grobj->setTextureBuffer(uvs);
    grobj->setNormalBuffer(normals);

    fclose(file);
}
