#include "Table.h"

namespace AOI
{
namespace SystemStore
{
    class UserTable : public Table
    {
    public:
        explicit UserTable(DatabasePtr const &database ) : Table( database )   {}
        virtual ~UserTable() {};
        
        enum FieldIndex
        {
            ID,
            NAME,
            PASSWORD,
            RESTRICTION,
            COUNT_,
        };

        /********
        * Table *
        ********/
        virtual String GetTableName()    const override { return StaticGetTableName(); }
        virtual int    GetFieldCount()   const override { return COUNT_; }
        virtual String GetFieldName(int) const override;
        virtual int    GetFieldBits(int) const override;
        virtual String GetFieldSql (int) const override;

        /*************
        * AlignmentTable *
        *************/
        static String StaticGetTableName();
    };
}
}