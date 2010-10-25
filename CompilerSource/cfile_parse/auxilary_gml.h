rnode *a = new rnode;
a.wid = 0;
a.hgt = 0;

list<unsigned int> unsorted;
for (int i = 0; i < GLYPHCOUNT; i++) {
  unsorted.push_back(rw[i]*rh[i] << 8 | i);
}

unsorted.sort();
pack_map(unsorted);
