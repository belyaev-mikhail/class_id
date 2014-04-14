
#include <cstdint>

namespace class_id {

class id {
    typedef const void* inner_t;
    inner_t value;
    
    id(inner_t value): value(value) {};
public:
    id() : value(0) {};
    id(const id& that): value(that.value) {};
    
    template<class T>
    static id get() {
        static const char unused = '\0';
        return id(static_cast<inner_t>(&unused));
    }
    
    template<class U>
    static id get(U&& u) {
        typedef typename std::remove_reference<U>::type Uplain;
        typedef typename std::remove_cv<Uplain>::type Unonconst;
        return get<Unonconst>();
    }
    
    friend std::ostream& operator<<(std::ostream& ost, id clsid) {
        return ost << "class_id#" << clsid.value;
    }
    
    friend bool operator==(id lhv, id rhv) {
        return lhv.value == rhv.value;
    }
    
    friend bool operator!=(id lhv, id rhv) {
        return lhv.value != rhv.value;
    }
    
    friend bool operator<(id lhv, id rhv) {
        return lhv.value < rhv.value;
    }
    
    bool valid() const { 
        return value != 0; 
    }

    size_t hash() const {
        return std::hash<inner_t>{}(value);
    }
};

template<class T>
id id_for(T&& value) {
    return id::get(std::forward<T>(value));
}

template<class T>
id id_for() {
    return id::get<T>();
}

} // namespace class_id

namespace std {
    template<>
    struct hash<class_id::id> {
        size_t operator ()(class_id::id clsid) const {
            return clsid.hash();
        }
    };
}

#if __cplusplus > 201103L
namespace class_id {
    
template<class T>
const id clazz = id::get<T>();

} /* namespace class_id */
#endif
