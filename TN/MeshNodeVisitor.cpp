#include "stdafx.h"

#include "MeshNodeVisitor.h"

#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Array>

void CMeshNodeVisitor::apply(Geode &geode)
{
    //���㵱ǰgeode�ڵ��Ӧ������任������������geode�ж����Ӧ����������
    osg::Matrix geodeMatrix = osg::computeLocalToWorld(getNodePath());

    unsigned int drwnum = geode.getNumDrawables();
    for (unsigned int i = 0; i < drwnum; i++)
    {
        ref_ptr<Geometry>geometry = geode.getDrawable(i)->asGeometry();
        if (!geometry)
            continue;

        for (unsigned int n = 0; n < geometry->getNumPrimitiveSets(); ++n)
        {
            PrimitiveSet* ps = geometry->getPrimitiveSet(n);
            if (!ps)
                continue;
            //��ȡ��������  
            ref_ptr<Vec3Array> va = dynamic_cast<Vec3Array*>(geometry->getVertexArray());
            //��ȡ��������  
            ref_ptr<Vec2Array> tex = dynamic_cast<Vec2Array*>(geometry->getTexCoordArray(0));

            if ((PrimitiveSet::DrawElementsUIntPrimitiveType == ps->getType()) && (PrimitiveSet::TRIANGLES == ps->getMode()))
            {
                ref_ptr<DrawElementsUInt>deui = dynamic_cast<DrawElementsUInt*>(ps);
                const unsigned indexNum = deui->getNumIndices(); //indexNum��ȡ�������ĸ���  216
                for (unsigned int m = 0; m < indexNum; m++)
                {
                    m_TriPoints->push_back(va->at(deui->at(m))*geodeMatrix);//��ȡ����λ�õĶ���  
                    if (NULL != tex)
                    {
                        m_TriTexCoordArray->push_back(tex->at(deui->at(m)));//��ȡ����λ�õ���������
                    }
                }
            }
            else
            {
                AfxMessageBox(L"ģ�ͻ��Ʒ�ʽ���������Σ�");
            }
        }
    }
    traverse(geode);
}



CMeshNodeVisitor::~CMeshNodeVisitor()
{}
