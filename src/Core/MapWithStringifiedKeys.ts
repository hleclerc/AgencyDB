
export default 
class MapWithStringifiedKeys<K,V> {
    get( key: K ): V {
        return this.map.get( key.toString() );
    }

    set( key: K, val: V ): MapWithStringifiedKeys<K,V> {
        this.map.set( key.toString(), val );
        return this;
    }

    delete( key: K ): MapWithStringifiedKeys<K,V> {
        this.map.delete( key.toString() );
        return this;
    }

    map = new Map<string,V>();
}