
namespace quick
{
  template<typename tp> struct double_pointer {
    tp **refd;
    inline tp *operator->() { return *refd; }
    inline tp &operator*() { return **refd; }
    inline const tp *operator->() const { return *refd; }
    inline const tp &operator*() const { return **refd; }
    inline double_pointer<tp> &operator=(tp* x) { *refd = x; return *this; }
    inline operator tp*() { return *refd; }
    template<typename tpto> inline operator tpto*() { return (tpto*)*refd; }
    inline bool operator==(tp *x) { return *refd == x; }
    inline bool operator!=(tp *x) { return *refd != x; }
    inline bool operator>=(tp *x) { return *refd >= x; }
    inline bool operator<=(tp *x) { return *refd <= x; }
    inline bool operator> (tp *x) { return *refd >  x; }
    inline bool operator< (tp *x) { return *refd <  x; }
    inline tp* operator~() { return *refd; }
    double_pointer(tp** to_ref): refd(to_ref) {}
  };
}
