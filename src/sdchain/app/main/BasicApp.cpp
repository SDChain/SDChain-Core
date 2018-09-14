//------------------------------------------------------------------------------
/*
    This file is part of sdchaind: https://github.com/SDChain/SDChain-core
    Copyright (c) 2017, 2018 SDChain Alliance.



*/
//==============================================================================

#include <BeastConfig.h>
#include <sdchain/app/main/BasicApp.h>
#include <sdchain/beast/core/CurrentThreadName.h>

BasicApp::BasicApp(std::size_t numberOfThreads)
{
    work_.emplace (io_service_);
    threads_.reserve(numberOfThreads);
    while(numberOfThreads--)
        threads_.emplace_back(
            [this, numberOfThreads]()
            {
                beast::setCurrentThreadName(
                    std::string("io_service #") +
                        std::to_string(numberOfThreads));
                this->io_service_.run();
            });
}

BasicApp::~BasicApp()
{
    work_ = boost::none;
    for (auto& _ : threads_)
        _.join();
}
