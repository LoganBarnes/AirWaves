// ////////////////////////////////////////////////////////////
// Created on 10/21/2017.
// Copyright (c) 2017. All rights reserved.
//  ___________________________$$$$$$$\__________
// | $$\    $$\                $$  __$$\      | ||
// |_$$ |___$$ |_$$\______$$\__$$ |__$$ |_____|_||
// | $$ |   $$ | $$$\    $$$ | $$$$$$$  |  () | ||
// |_\$$\__$$  |_$$$$\__$$$$ |_$$  ____/______|_||
// |  \$$\$$  /  $$\$$\$$ $$ | $$ |        () | ||
// |___\$$$  /___$$ \$$$  $$ |_$$ |___________|_||
// |    \$  /    $$ |\$  /$$ | \__|           | ||
// |_____\_/_____$$ |_\_/_$$ |________________|_||
//               \__|     \__|
// The Visual Music Project
// Created by Logan Barnes
// ////////////////////////////////////////////////////////////
#include "VMP.hpp"
#include <memory>

namespace vmp
{
struct SeanEntity
{
private:
    template<typename T>
    struct Entity;

public:
    template<typename T>
    explicit SeanEntity(T entity)
        : self_{std::make_unique<Entity<T >>(std::move(entity))}
    {}

    void update(double t, double dt)
    {
        self_->update(t, dt);
    }

private:
    struct UpdateEntity
    {
        virtual ~UpdateEntity() = default;
        virtual void update(double, double) = 0;
    };

    template<typename T>
    struct Entity: public UpdateEntity
    {
        explicit Entity(T entity)
            : data_{std::move(entity)}
        {}

        void update(double t, double dt) final
        {
            data_.update(t, dt);
        }
        T data_;
    };

    std::unique_ptr<UpdateEntity> self_;
};
}
void VMP::resume()
{
    vmp::MainStream::instance().start_stream();
}
void VMP::pause()
{
    vmp::MainStream::instance().stop_stream();
}
void VMP::reset()
{
    vmp::MainStream::instance().stop_stream();
}
bool VMP::is_paused()
{
    return !vmp::MainStream::instance().is_stream_running();
}
