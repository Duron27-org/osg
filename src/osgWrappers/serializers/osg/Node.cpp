#include <osg/Node>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

// _initialBound
static bool checkInitialBound( const osg::Node& node )
{
    return node.getInitialBound().valid();
}

static bool readInitialBound( osgDB::InputStream& is, osg::Node& node )
{
    osg::Vec3d center;
    double radius;
    is >> is.BEGIN_BRACKET;
    is >> is.PROPERTY("Center") >> center;
    is >> is.PROPERTY("Radius") >> radius;
    is >> is.END_BRACKET;
    node.setInitialBound( osg::BoundingSphere(center, radius) );
    return true;
}

static bool writeInitialBound( osgDB::OutputStream& os, const osg::Node& node )
{
    const osg::BoundingSphere& bs = node.getInitialBound();
    os << os.BEGIN_BRACKET << osgDB::OutputStream::Endl{};
    os << os.PROPERTY("Center") << osg::Vec3d(bs.center()) << osgDB::OutputStream::Endl{};
    os << os.PROPERTY("Radius") << double(bs.radius()) << osgDB::OutputStream::Endl{};
    os << os.END_BRACKET << osgDB::OutputStream::Endl{};
    return true;
}

// _descriptions
static bool checkDescriptions( const osg::Node& node )
{
    return node.getDescriptions().size()>0;
}

static bool readDescriptions( osgDB::InputStream& is, osg::Node& node )
{
    unsigned int size = is.readSize(); is >> is.BEGIN_BRACKET;
    for ( unsigned int i=0; i<size; ++i )
    {
        std::string value;
        is.readWrappedString( value );
        node.addDescription( value );
    }
    is >> is.END_BRACKET;
    return true;
}

static bool writeDescriptions( osgDB::OutputStream& os, const osg::Node& node )
{
    const osg::Node::DescriptionList& slist = node.getDescriptions();
    os.writeSize(slist.size()); os << os.BEGIN_BRACKET << osgDB::OutputStream::Endl{};
    for ( osg::Node::DescriptionList::const_iterator itr=slist.begin();
          itr!=slist.end(); ++itr )
    {
        os.writeWrappedString( *itr );
        os << osgDB::OutputStream::Endl{};
    }
    os << os.END_BRACKET << osgDB::OutputStream::Endl{};
    return true;
}

struct NodeGetOrCreateStateSet : public osgDB::MethodObject
{
    virtual bool run(void* objectPtr, osg::Parameters& inputParameters, osg::Parameters& outputParameters) const
    {
        osg::Node* node = reinterpret_cast<osg::Node*>(objectPtr);
        outputParameters.push_back(node->getOrCreateStateSet());
        return true;
    }
};


REGISTER_OBJECT_WRAPPER( Node,
                         new osg::Node,
                         osg::Node,
                         "osg::Object osg::Node" )
{
    ADD_USER_SERIALIZER( InitialBound );  // _initialBound
    ADD_OBJECT_SERIALIZER( ComputeBoundingSphereCallback,
                           osg::Node::ComputeBoundingSphereCallback, NULL );  // _computeBoundCallback
    ADD_OBJECT_SERIALIZER( UpdateCallback, osg::Callback, NULL );  // _updateCallback
    ADD_OBJECT_SERIALIZER( EventCallback, osg::Callback, NULL );  // _eventCallback
    ADD_OBJECT_SERIALIZER( CullCallback, osg::Callback, NULL );  // _cullCallback
    ADD_BOOL_SERIALIZER( CullingActive, true );  // _cullingActive
    ADD_HEXINT_SERIALIZER( NodeMask, 0xffffffff );  // _nodeMask

    ADD_USER_SERIALIZER( Descriptions );  // _descriptions, deprecated
    {
        UPDATE_TO_VERSION_SCOPED( 77 )
        REMOVE_SERIALIZER( Descriptions );
    }

    ADD_OBJECT_SERIALIZER( StateSet, osg::StateSet, NULL );  // _stateset

    ADD_METHOD_OBJECT( "getOrCreateStateSet", NodeGetOrCreateStateSet );
}
