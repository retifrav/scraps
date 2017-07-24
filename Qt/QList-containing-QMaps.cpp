QMap<Key, Type> map;
QList< QMap<Key, Type> > list;
list.append(map);
 
// or

typedef QMap<Key, Type> MyMap;
MyMap map;
QList<MyMap> list;
list.append(map);