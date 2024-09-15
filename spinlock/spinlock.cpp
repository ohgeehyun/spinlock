// spinlock.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

/*
    싱글코어 환경에서 스핀락을 사용하는것은 의미가없다. 결국cpu코어를 사용(반복문)하여 락을 기다리기 때문이다.
    but 듀얼 코어환경에서는 컨텍스트 스위칭보다 cpu자원을 사용하는것이 좋은 환경일때는 뮤텍스 보다 좋은 선택이 될 수도 있다.
    현대 운영체제에서는 하이브리드 뮤텍스가 결국 처음에는 락을돌리다 일정시간 지나면 그제서야 스레드를 대기상태로 만들기는한다.
    결국 왠만해서는 뮤텍스를 사용하는게 맞지만 그래도 어떻게 돌아가는지 공부하자.
*/

#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> lock(false); // 원자성을 보장 이 데이터를 사용하는 작업 중 일떄는 작업이 영원히 멈추거나 이 작업만 하거나 

void critical_section(int id) {
    
    while (lock.exchange(true, std::memory_order_acquire)) { // 해당 명령 뒤에 오는 모든 메모리 명령들이 해당 명령 위로 재배치되는 것을 금지
        // Spin while the lock is held by another thread
    }

    std::cout << "Thread " << id << " entered critical section" << '\n';

    // Do some work in the critical section
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (int i = 0; i < 50; ++i) {
        std::cout << "test number : " << i << '\n';
    }
    std::cout << "Thread " << id << " end 작업 끝" <<'\n';
    lock.store(false, std::memory_order_release); // 스레드간 메모리 접근을 제어하기 위해 사용
                                                  // 1.락을 해제하기 전에 수행된 모든 메모리 접근이 완료되었음을 보장
                                                  // 2.락을 해제한 후 다른 스레드가 락을 흭득하고 임계 영역에 들어갈 떄 이전에 수행된 메모리 작업들이 보이도록 함
}

int main() {
    int j = 0;

    std::thread t1(critical_section, 1);
    std::thread t2(critical_section, 2);

    t1.join();
    t2.join();

    system("pause");

    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
