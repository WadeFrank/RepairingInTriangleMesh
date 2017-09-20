/*!
 * findNodeVisitor.h
 * date: 2017/05/02 17:43
 *
 * author: dav1sNJU
 *
 * brief: 
 *
 *
*/
#pragma once
#include <osg/NodeVisitor>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/MatrixTransform>

using namespace osg;

typedef std::vector<osg::Node*> nodeListType;

class CfindNodeVisitor :
    public NodeVisitor
{
public:
    // Ĭ�ϵĹ��캯�������ҽڵ���Ϊ�գ���������Ϊ�����ӽڵ� 
    CfindNodeVisitor();
    ~CfindNodeVisitor();
    // ���ڵ����Ĺ��캯�������ҽڵ���ΪsearchName��������Ϊ�����ӽڵ� 
    CfindNodeVisitor(const std::string &searchName); 
    // ���������ҽڵ���ƥ��Ľڵ���ӵ��ڵ��б� 
    virtual void apply(osg::Node &searchNode); 
    // �����û����ҵĽڵ���  
    void setNameToFind(const std::string &searchName); 
    // ���ؽڵ��б��е�һ���ڵ��ָ��
    osg::Node* getFirst();
    // ���ؽڵ��б������һ���ڵ��ָ��
    osg::Node* getLast();
    // ���ؽڵ��б��һ������ 
    nodeListType& getNodeList() { return foundNodeList; };
    // ���ؽڵ���Ŀ
    int getNodeCnt() { return foundNodeList.size(); };
private:
    // ���ҵĽڵ����� 
    std::string searchNodeName;
    // �ڵ��б�  
    nodeListType foundNodeList; 
};

