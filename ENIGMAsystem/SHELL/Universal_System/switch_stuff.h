namespace enigma
{
  int switch_hash(variant x) {
    if (!x.type)
      if (x == int(x))
        return int(x);
      else
        return int(x * 65536);
    else
    {
      int ret = 0;
      const string& n = x.sval;
      for (size_t i = 0; i < n.length(); i++)
        ret = 31*ret + n[i];
      return ret;
    }
  }
}
