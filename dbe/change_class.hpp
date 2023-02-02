/*
 * change_class.hpp
 *
 *  Created on: Nov 10, 2015
 *      Author: lgeorgop
 */

#ifndef DBE_CHANGE_CLASS_HPP_
#define DBE_CHANGE_CLASS_HPP_

#include "confaccessor.h"
#include "config_api_set.h"
#include "Command.h"
#include "Exceptions.h"
#include "messenger.h"

#include "config/ConfigObject.hpp"
#include "config/Errors.hpp"
#include "config/Schema.hpp"

#include <QObject>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>

#include <string>

namespace dbe
{
namespace actions
{
//------------------------------------------------------------------------------------------
template<typename T>
class ChangeClass:
  public onobject
{
public:
  ChangeClass ( tref Object, daq::config::attribute_t AttributeData, T NewValueData,
                QUndoCommand * parent = nullptr );
  void undo();
  void redo();

private:
  T OldValue;
  T NewValue;
  daq::config::attribute_t Attribute;
  bool Success;
};

template<typename T>
ChangeClass<T>::ChangeClass ( tref Object, daq::config::attribute_t AttributeData,
                              T NewValueData, QUndoCommand * parent )
  : onobject ( Object, parent ),
    NewValue ( NewValueData ),
    Attribute ( AttributeData ),
    Success ( true )
{
  try
  {
    QStringList Data
    { dbe::config::api::get::attribute::list<QStringList> ( Object, Attribute ) };
    OldValue = convert::to<T> ( Data );
  }
  catch ( daq::config::Exception const & )
  {
  }

  setText (
    QObject::tr ( "Attribute %1 of object %2 was updated." ).arg (
      AttributeData.p_name.c_str() ).arg ( Object.UID().c_str() ) );
}

template<typename T>
void ChangeClass<T>::undo()
{
  try
  {
    if ( isvalid() )
    {
      failed();
      dbe::config::api::set::noactions::aclass ( this->checkedref(), Attribute, OldValue );
      toggle();
    }
  }
  catch ( daq::config::Exception const & e )
  {
    Success = false;
    throw daq::dbe::ObjectChangeWasNotSuccessful ( ERS_HERE, e );
  }
  catch ( daq::dbe::config_object_retrieval_result_is_null const & ex )
  {
    FAIL ( "Operation did not complete because a lookup in the underlying database failed",
           ex.what() );
    throw daq::dbe::ObjectChangeWasNotSuccessful ( ERS_HERE, ex );
  }

}

template<typename T>
void ChangeClass<T>::redo()
{
  try
  {
    if ( isvalid() )
    {
      failed();
      QStringList Data
      { dbe::config::api::get::attribute::list<QStringList> ( this->checkedref(), Attribute ) };
      OldValue = convert::to<T> ( Data );

      dbe::config::api::set::noactions::aclass ( this->checkedref(), Attribute, NewValue );
      toggle();
    }
  }
  catch ( daq::config::Exception const & e )
  {
    Success = false;
    throw daq::dbe::ObjectChangeWasNotSuccessful ( ERS_HERE, e );
  }
  catch ( daq::dbe::config_object_retrieval_result_is_null const & ex )
  {
    FAIL ( "Operation did not complete because a lookup in the underlying database failed",
           ex.what() );
    throw daq::dbe::ObjectChangeWasNotSuccessful ( ERS_HERE, ex );
  }

}

}
}
#endif // DBE_CHANGE_CLASS_HPP_
