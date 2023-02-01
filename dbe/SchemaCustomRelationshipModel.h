#ifndef CUSTOMRELATIONSHIPMODEL_H
#define CUSTOMRELATIONSHIPMODEL_H

/// Include oks
#include <oks/class.h>
#include <oks/relationship.h>
/// Including Schema
#include "SchemaCustomModelInterface.h"

namespace dbse
{

class CustomRelationshipModel: public CustomModelInterface
{
public:
  CustomRelationshipModel ( OksClass * ClassInfo, QStringList Headers, bool Derived = false );
  ~CustomRelationshipModel();
  void setupModel();
private:
  OksClass * SchemaClass;
  bool SchemaDerived;
};

}  // namespace dbse
#endif // CUSTOMRELATIONSHIPMODEL_H
