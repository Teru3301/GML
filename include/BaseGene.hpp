
class Gene 
{
public:
    virtual ~Gene() = default;
    virtual void Run() = 0;
    virtual void Mutate() = 0;

private:

};

