#pragma once
#include <osg/NodeVisitor>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/MatrixTransform>

using namespace osg;

class CMeshNodeVisitor :
    public NodeVisitor
{
public:
    //���캯��    
    CMeshNodeVisitor() :NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN)
    {
        m_TriPoints = new Vec3Array;   //�������飬���ڱ��������Ŷ�Ӧ�Ķ���  
        m_TriTexCoordArray = new Vec2Array;
    }
    ~CMeshNodeVisitor();

    //����MatrixTransform�ڵ��apply����  
    virtual void apply(MatrixTransform &node)
    {
        traverse(node);
    }

    //����group��apply()����  
    virtual void apply(Group &group)
    {
        traverse(group);
    }

    //����Geode��apply()��������ȡ��Ӧ�����ŵĶ�������m_TriPoints����������������triangle����������������centerPoint  
    void CMeshNodeVisitor::apply(Geode &geode);

public:

    ref_ptr<Vec3Array> getTriPoints()
    {
        return m_TriPoints.release();
    }
    ref_ptr<Vec2Array> getTriTexCoordArray()
    {
        return m_TriTexCoordArray.release();
    }

private:
    ref_ptr<DrawElementsUInt>deui;
    ref_ptr<Vec3Array> m_TriPoints;
    ref_ptr<Vec2Array> m_TriTexCoordArray;
};

