// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGame.h"
#include "Paddle.h"
#include "RemotePaddleController.h"

#include <msgpack.hpp>
#include <sstream>


ARemotePaddleController::ARemotePaddleController()
{
}

void ARemotePaddleController::BeginPlay()
{
    Super::BeginPlay();
}

void ARemotePaddleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* pawn = GetPawn();
    if (!pawn) {
        return;
    }

    APaddle* paddle = Cast<APaddle>(GetPawn());
    if (!paddle) {
        return;
    }

    paddle->MovementDirection = Communicate(paddle);
}

struct Message
{
    int episodeStep;
    int cpuScore;
    int playerScore;

    int height;
    int width;

    std::vector<char> screen;

    MSGPACK_DEFINE(episodeStep, cpuScore, playerScore, height, width, screen);
};

Action ARemotePaddleController::Communicate(const State& state) {
    auto* gameMode = state->GameMode;
    auto screenshot = gameMode->ScreenCapturer->Screenshot;

    if (screenshot.size() > 100000) {
        UE_LOG(LogTemp, Warning, TEXT("Screenshot is too large: %d bytes"), screenshot.size());
        return 0.0;
    }

    Message message;
    message.episodeStep = gameMode->EpisodeStep;
    message.cpuScore = gameMode->CpuScore;
    message.playerScore = gameMode->PlayerScore;
    message.height = gameMode->ScreenCapturer->Height;
    message.width = gameMode->ScreenCapturer->Width;
    message.screen = screenshot;

    try {
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, message);

        auto action = relay.Act(sbuf.data(), sbuf.size());

        msgpack::object_handle oh = msgpack::unpack(action.data(), action.size());
        msgpack::object deserialized = oh.get();

        float dst;
        deserialized.convert(dst);

        UE_LOG(LogTemp, Warning, TEXT("Deserialized value: %f"), dst);

        return dst;
    } catch (std::exception& e) {
        FString error_message(e.what());
        UE_LOG(LogTemp, Warning, TEXT("Caught an exception: %s"), *error_message);

        return 0.0;
    }
}
