#include <vector>
#include <string>

class MenuElement
{
public:

//Constructors
MenuElement();
MenuElement(const std::string &name_);

//Elements access
MenuElement& operator[](const std::string &child_);
MenuElement& operator[](int child_);

//Configuring
MenuElement& setAction(int action_);

//Get element info
const std::string& get_name() const;
int getAction() const;
bool hasChildren() const;
int childrenAmount() const;
int getSelected();

//Menu movement
MenuElement& setSelectedElement(int selectedElement_);
void onBack();
void onForward();
MenuElement* onConfirm();

private:
std::vector<MenuElement> m_elements;
std::string m_name;
int m_selectedElement = 0;
int m_action = 0;

void clampSelection();

};