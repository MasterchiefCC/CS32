void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if(path.size()!=0)path += "/" + m->name();
	else path += m->name();
	if (path.size() != 0)cout << path << endl;
	if (m->menuItems() == NULL||m->menuItems()->size()==0)return;
	for (vector<MenuItem*>::const_iterator mit = m->menuItems()->begin(); mit != m->menuItems()->end(); mit++) {
		const MenuItem * temp = (*mit);
		listAll(temp, path);
	}
}
