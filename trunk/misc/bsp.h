/*******************************************************************
 *   Title: BSP.h
 *    Desc: Excessively generic implementation of a 
 *          node-based BSP tree.
 * copyright (c) 2000 by Adrian Perez
 ******************************************************************/

/***
 REVISION HISTORY

  11/02/00 : Cuban
    Gave adaptor control over HP selection, added serialization
    support using the iostream library.
 ***/

#include <vector>
#include <istream>
#include <ostream>
#include <assert.h>


// One of the inputs to the tree constructor is an adaptor that
// implements the three operations required to build
// a BSP tree:
// 
// (*) Classifying an element against a hyperplane 
//       (ex: polygon->plane testing)
// (*) Splitting an element by a hyperplane 
//       (ex: polygon->plane => front/back pieces)
// (*) Choosing a hyperplane to partition a set of elements,
//       given said set of elements
// 
// struct sTreeAdaptor
// {
//     eBspRelation Classify( 
//         const T_hyperplane& plane, 
//         const T_element& elem ) const;
//
//     void Split( 
//         const T_hyperplane& plane, 
//         const T_element& elem, 
//         T_element* pFront,
//         T_element* pBack ) const;
// 
//     void ChooseHyperplane( 
//         std::vector<T_element>& toProcess, 
//         T_hyperplane* pPlane ) const;
// };


// Returned by the Classify method of the tree adaptor.
enum eBspRelation
{
    BSP_RELAT_IN_FRONT,
    BSP_RELAT_IN_BACK,
    BSP_RELAT_SPLIT,
    BSP_RELAT_COPLANAR
};


// This is a non-brep tree.  Used just for classification of
// polygons.  No leaves.
template< class T_element, class T_hyperplane, class T_treeAdaptor >
class cNodeBSP
{
    // Other objects need to see what a node looks like.
public:
    struct sNode
    {
        typedef std::vector< T_element > elemVec;

        sNode* m_pFront;
        sNode* m_pBack;

        elemVec m_contents;
        T_hyperplane m_hp;

        sNode( elemVec& toProcess, const T_treeAdaptor& adap )
        : m_pFront( NULL )
        , m_pBack( NULL )
        {
            // Setup
            elemVec frontVec, backVec;
            frontVec.reserve( toProcess.size() );
            backVec.reserve( toProcess.size() );

            // Choose which node we're going to use.
            adap.ChooseHyperplane( toProcess, &m_hp );

            // Iterate across the rest of the polygons
            elemVec::iterator iter = toProcess.begin();
            for( ; iter != toProcess.end(); ++iter )
            {
                T_element front, back;
                switch( adap.Classify( m_hp, *iter ) )
                {
                case BSP_RELAT_IN_FRONT:
                    frontVec.push_back( *iter );
                    break;
                case BSP_RELAT_IN_BACK:
                    backVec.push_back( *iter );
                    break;
                case BSP_RELAT_COPLANAR:
                    m_contents.push_back( *iter );
                    break;
                case BSP_RELAT_SPLIT:
                    adap.Split( m_hp, *iter, &front, &back );
                    frontVec.push_back( front );
                    backVec.push_back( back );
                    break;
                default: break;
                }
            }

            // Now recurse if necessary
            if( !frontVec.empty() )
                m_pFront = new sNode( frontVec, adap );
            if( !backVec.empty() )
                m_pBack = new sNode( backVec, adap );
        }

        sNode( std::istream& in )
        {
            // First char is the child state 
            // (0x1 means front child, 0x2 means back child)
            int childState;
            in >> childState;

            // Next is the hyperplane for the node
            in >> m_hp;

            // Next is the number of elements in the node
            unsigned int nElem;
            in >> nElem;
            m_contents.reserve( nElem );

            while( nElem-- )
            {
                T_element elem;
                in >> elem;
                m_contents.push_back( elem );
            }

            // recurse if we have children.
            if( childState & 0x1 )
                m_pFront = new sNode( in );
            else
                m_pFront = NULL;
            if( childState & 0x2 )
                m_pBack = new sNode( in );
            else
                m_pBack = NULL;
        }

        void Write( std::ostream& out )
        {
            // What is our child state?
            int childState = 0;
            if( m_pFront ) childState += 1;
            if( m_pBack ) childState += 2;

            // write it out
            out << childState << " ";

            // Write out hp
            out << m_hp << " ";

            // now take care of the contents
            unsigned int nElem = m_contents.size();
            out << nElem << " ";
            while( nElem-- )
                out << m_contents[nElem] << " ";

            // Finally deal with the children
            if( m_pFront )
                m_pFront->Write( out );
            if( m_pBack )
                m_pBack->Write( out );
        }

        ~sNode()
        {
            delete m_pFront;
            delete m_pBack;
        }
    };
protected:

    sNode* m_pRoot;
    T_treeAdaptor m_adap;

    std::vector< T_element > m_toBuild;

public:

    cNodeBSP( T_treeAdaptor& adaptor = T_treeAdaptor() )
    : m_adap( adaptor )
    , m_pRoot( NULL )
    {}

    sNode* GetRoot(){ return m_pRoot; }

    void AddElement( const T_element& elem )
    {
        bool bTreeAlreadyBuilt = (m_pRoot != NULL);
        assert( !bTreeAlreadyBuilt );

        m_toBuild.push_back( elem );
    }

    void BuildTree()
    {
        assert( m_toBuild.size() );
        m_pRoot = new sNode( m_toBuild, m_adap );
    }

    void Read( std::istream& in )
    {
        // Shouldn't read in over a tree.
        assert( !m_pRoot );

        m_pRoot = new sNode( in );
    }

    void Write( std::ostream& out )
    {
        // Shouldn't write out a null tree
        assert( m_pRoot );

        m_pRoot->Write( out );
    }
};

/* As an example, with a completed tree that used my polygon structure as
   an element and a plane as a hyperplane, this is a function that was
   written to do inorder traversal, initially called with the root of the
   tree (s3DTreeAdaptor not included for brevity):

typedef cNodeBSP< polygon<point3>, plane3, s3DTreeAdaptor > polyTree;

void InOrderTraversal( polyTree::sNode* pNode, point3 camLoc )
{
    std::vector< polygon<point3> >::iterator iter=pNode->m_contents.begin();
    switch( pNode->m_hp.TestPoint( camLoc ) )
    {
    case ptFront:
        if( pNode->m_pBack ) InOrderTraversal( pNode->m_pBack, camLoc );

        for( ; iter != pNode->m_contents.end(); ++iter )
            DrawPoly( *iter );

        if( pNode->m_pFront ) InOrderTraversal( pNode->m_pFront, camLoc );
        break;
    default:
        if( pNode->m_pFront ) InOrderTraversal( pNode->m_pFront, camLoc );

        for( ; iter != pNode->m_contents.end(); ++iter )
            DrawPoly( *iter );

        if( pNode->m_pBack ) InOrderTraversal( pNode->m_pBack, camLoc );
        break;
    }
};

*/
