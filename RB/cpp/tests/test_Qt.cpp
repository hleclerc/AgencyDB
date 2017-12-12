#include <RiotBase/Variable/Symbol/Graphviz.h>
#include <RiotBase/Qt/TextEdit.h>
#include <RiotBase/Qt/Slider.h>
#include <RiotBase/Db/Gev.h>
#include <RiotBase/Db/Db.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextEdit>

using namespace RiotBase;

struct Inst {
    Inst( int dev, Db::InitialInstance ii ) : db( dev, KnownInodes::root_usr, ii ) {
        db.listen( "my_socket" );
        bind();

        *s = "ABCDF\nGHIJKL";
        *n = 50;
    }
    Inst( int dev ) : db( dev, KnownInodes::root_usr ) {
        db.connect( "my_socket", "root", "pouet" );
        bind();
    }

    void bind() {
        n = (Number *)db.bind( "/my_number" );
        s = (String *)db.bind( "/my_string" );
        Directory d = db.bind( "/" );

        //        if ( db.get_dev_id() == 101 ) {
        //            Graphviz gr;
        //            gr << n->rp;
        //            gr << s->rp;
        //            gr << d.rp;
        //            gr.display();
        //        }



        s += "";
        s.find();

        QVBoxLayout *qvb = new QVBoxLayout;
        qvb->addWidget( new Slider  ( n.ptr() ) );
        qvb->addWidget( new TextEdit( s.ptr() ) );
        qvb->addWidget( new QTextEdit );

        QWidget *window = new QWidget;
        window->setLayout( qvb );
        window->show();
    }

    Db              db;
    AutoPtr<Number> n;
    AutoPtr<String> s;
};

int main( int argc, char **argv ) {
    QApplication qapp( argc, argv );

    Vec<AutoPtr<Inst>> instances;
    instances << new Inst( 100, Db::InitialInstance() );
    for( int i = 1; i < 3; ++i )
        instances << new Inst( 100 + i );

    gev->add_timer( [](int){ QApplication::processEvents(); return true; }, 0.01 );
    gev->run();
}
