#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>
#include <limits>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <sstream>

// �ùķ��̼� �ð� ����
const int SIMULATION_START_TIME = 6 * 60; // 7�� (�� ����)
const int SIMULATION_END_TIME = 10 * 60;   // 9�� (�� ����)
const int TIME_STEP = 1;                  // 1�� ����

// ��ũ Ŭ���� ����
class Link {
public:
    // ������
    Link(std::string n, std::string t, double fft, int cap, double cf, double exp)
        : name(n), type(t), free_flow_time(fft), capacity(cap),
        congestion_factor(cf), exponent(exp), average_traffic(0.0) {
        // �ùķ��̼� �ð��� ���� �ð� ���� �ʱ�ȭ
        int total_time_slots = SIMULATION_END_TIME - SIMULATION_START_TIME + 1;
        time_slots.resize(total_time_slots);
    }

    // �ð� ���� �ʱ�ȭ �Լ�
    void reset_time_slots() {
        for (auto& slot : time_slots) {
            slot.clear();
        }
    }

    // Ư�� �ð��뿡 ������Ʈ �߰�
    void add_agent_to_time_slot(int time_slot, int agent_id) {
        time_slots[time_slot].push_back(agent_id);
    }

    // ������Ʈ�� ��ũ�� �ִ� ������ ��� ȥ�⵵ ���
    double calculate_average_congestion(int start_time, int end_time) {
        double total_traffic = 0.0;
        int time_count = 0;

        for (int t = start_time; t < end_time; ++t) {
            int slot = t - SIMULATION_START_TIME;
            if (slot >= 0 && slot < time_slots.size()) {
                int current_traffic = time_slots[slot].size();
                total_traffic += current_traffic;
                ++time_count;
            }
        }

        if (time_count == 0) return 0.0;

        return total_traffic / time_count;
    }

    // **�߰��� �Լ�: �ùķ��̼� ��ü �Ⱓ ������ ��� ���뷮 ���**
    void calculate_overall_average_traffic() {
        double total_traffic = 0.0;
        int time_count = 0;

        for (const auto& slot : time_slots) {
            total_traffic += slot.size();
            ++time_count;
        }

        if (time_count == 0) {
            average_traffic = 0.0;
        }
        else {
            average_traffic = total_traffic / time_count;
        }
    }

    int get_capacity()
    {
        return capacity;
    }

    void set_capacity(int cap)
    {
        capacity = cap;
    }

    double get_congestion_factor()
    {
        return congestion_factor;
    }

    void set_congestion_factor(double cf)
    {
        congestion_factor = cf;
    }

    double get_free_flow_time()
    {
        return free_flow_time;
    }
    void set_free_flow_time(double T0)
    {
        free_flow_time = T0;
    }

    double get_exponent()
    {
        return exponent;
    }
    void set_exponent(double exp)
    {
        exponent = exp;
    }

    std::string get_name()
    {
        return name;
    }

    std::string get_type()
    {
        return type;
    }

    int get_time_slot_size() {
        return time_slots.size();
    }

    double get_average_traffic()
    {
        return average_traffic;
    }
private:
    std::string name;
    std::string type;
    double free_flow_time;   // ���� �帧 �ð� (�� ����)
    int capacity;
    double congestion_factor;
    double exponent;

    // �ð��� ������Ʈ ���� �����ϴ� ����
    std::vector<std::vector<int>> time_slots; // [�ð�][������Ʈ ID ���]

    // **�߰��� ��� ����: average_traffic**
    double average_traffic;

};

// Location Ŭ���� ����
class Location {
public:
    Location(std::string n, int cap)
        : name(n), capacity(cap), current_agents(0) {}

    void add_agent() {
        if (current_agents < capacity) {
            current_agents++;
        }
    }

    void remove_agent() {
        if (current_agents > 0) {
            current_agents--;
        }
    }

    bool has_capacity() {
        return current_agents < capacity;
    }

    void reset_agents() {
        current_agents = 0;
    }

    int get_capactiy()
    {
        return capacity;
    }

    void set_capacity(int cap)
    {
        capacity = cap;
    }

    std::string get_name()
    {
        return name;
    }

    int get_current_agents()
    {
        return current_agents;
    }

private:
    std::string name;
    int capacity;
    int current_agents;
};

// ������Ʈ Ŭ����
class Agent {
public:
    Agent(Location* h, Location* w, std::vector<std::vector<Link*>> routes, std::vector<Location*> origins, double pat, double start, std::mt19937& g, double beta_k)
        : home(h), work(w), possible_routes(routes), possible_origins(origins), PAT(pat * 60),
        start_time(start * 60), prev_start_time(start * 60), beta_r(-0.27), beta_t(-0.06), beta_j(-0.05), beta_k(beta_k),
        zeta(0.9), gamma(20.0), delta(5.0), lambda(4.0), vtts(1.0), prev_utility(0.0), gen(g), rand_dist(0.0, 1.0),
        travel_time(0.0), arrival_time(0.0), SDE(0), SDL(0), travel_time_utility(0),
        punctuality_utility(0), location_utility(0), total_utility(0), habit_utility(0), habit(0), isChangeHabit(false),
        previous_travel_times(0.0)
    {

        random_utility_factor = rand_dist(gen) * 0.2;

        // �ʱ� ��θ� �����ϰ� ����
        if (!possible_routes.empty()) {
            std::uniform_int_distribution<> route_dist(0, possible_routes.size() - 1);
            int random_route_index = route_dist(gen);
            current_route = possible_routes[random_route_index];
        }
    }

    // ���� �ð� �� ���� �ð� �ʱ�ȭ �Լ�
    void reset_travel_info() {
        travel_time = 0.0;
        arrival_time = 0.0;
    }

    double get_previous_travel_times() const
    {
        return previous_travel_times;
    }

    void set_previous_travel_times(double pre_time)
    {
        previous_travel_times = pre_time;
    }

    double get_start_time() const
    {
        return start_time;
    }

    void set_start_time(double time)
    {
        start_time = time;
    }

    double get_arrival_time() const
    {
        return arrival_time;
    }

    void set_arrival_time(double time)
    {
        arrival_time = time;
    }

    std::vector<Link*> get_current_route() const
    {
        return current_route;
    }

    void set_current_route(std::vector<Link*> link)
    {
        current_route = link;
    }


    double get_travel_time() const
    {
        return travel_time;
    }

    void set_travel_time(double time)
    {
        travel_time = time;
    }

    void update_travel_time(double time)
    {
        travel_time += time;
    }

    double get_prev_utility() const
    {
        return prev_utility;
    }

    void add_habit()
    {
        habit++;
    }

    Location* get_home() const
    {
        return home;
    }

    void set_home(Location* new_home)
    {
        home = new_home;
    }

    Location* get_work() const
    {
        return work;
    }

    int get_habit()
    {
        return habit;
    }


    void set_habit(int h)
    {
        habit = h;
    }

    void down_upate_habit(int h)
    {
        habit -= h;
    }

    void set_beta_k(double k)
    {
        beta_k = k;
    }

    bool get_isChangeHabit()
    {
        return isChangeHabit;
    }

    void set_isChangeHabit(bool result)
    {
        isChangeHabit = result;
    }

    double get_prev_utility()
    {
        return prev_utility;
    }

    std::vector<std::vector<Link*>> get_possible_routes()
    {
        return possible_routes;
    }


    void calc_agent_utility(int mode)
    {
        // ��ƿ��Ƽ ���
        double travel_time_hours = travel_time; // 60.0;
        double arrival_time_hours = arrival_time; // 60.0;
        double PAT_hours = PAT; // 60.0;

        travel_time_utility = vtts * travel_time_hours * beta_r;

        SDE = std::max(PAT_hours - arrival_time_hours, 0.0);
        SDL = std::max(arrival_time_hours - PAT_hours, 0.0);
        punctuality_utility = (zeta * SDE + gamma * SDL + (arrival_time_hours > PAT_hours ? delta : 0)) * beta_t;

        double occupancy_rate = static_cast<double>(home->get_current_agents()) / home->get_capactiy();
        location_utility = std::exp(lambda * occupancy_rate) * beta_j;

        if (mode == 1 || mode == 2)
        {
            habit_utility = std::clamp(beta_k * habit, 0.0, beta_k * 5);
        }
        else if (mode == 3)
            habit_utility = std::clamp(beta_k * habit, beta_k * 5, 0.0);
        else
            habit_utility = 0;        

        total_utility = travel_time_utility + punctuality_utility + location_utility + habit_utility;

        prev_utility = total_utility;
    }

    void agent_log_agent_utilitie(int agent_id, std::ofstream& agent_utility_file, int iter)
    {
        std::string route_str;
        for (auto& link : current_route) {
            if (!route_str.empty()) route_str += " -> ";
            route_str += link->get_name();
        }

        agent_utility_file << iter << ","
            << agent_id << ","
            << home->get_name() << ","
            << work->get_name() << ","
            << get_start_time() / 60.0 << "," // �ð� ������ ��ȯ�Ͽ� ���
            << route_str << ","
            << travel_time << ","
            << SDE << ","
            << SDL << ","
            << travel_time_utility << ","
            << punctuality_utility << ","
            << location_utility << ","
            << habit_utility << ","
            << total_utility << "\n";
    }

private:
    // ��� ����
    Location* home;
    Location* work;
    std::vector<std::vector<Link*>> possible_routes;
    std::vector<Location*> possible_origins;
    std::vector<Link*> current_route;
    double PAT; // ��ȣ ���� �ð� (�� ����)
    double start_time; // ��� �ð� (�� ����)
    double beta_r;
    double beta_t;
    double beta_j;
    double beta_k; // ������ ���� ���
    double zeta;
    double gamma;
    double delta;
    double lambda;
    double vtts;
    double random_utility_factor;
    double prev_start_time;
    double prev_utility;
    std::mt19937& gen;

    std::uniform_real_distribution<> rand_dist;

    int habit;
    bool isChangeHabit;

    double travel_time;
    double arrival_time; // ���� �ð�

    // ��ƿ��Ƽ ���� ����
    double SDE;
    double SDL;
    double travel_time_utility;
    double punctuality_utility;
    double location_utility;
    double total_utility;
    double habit_utility;

    double previous_travel_times;
};

// ��� ��� ã�� �Լ�
void find_all_routes(
    const std::string& current_node,
    const std::string& destination,
    std::unordered_map<std::string, std::vector<std::pair<std::string, Link*>>>& graph,
    std::vector<Link*>& current_route,
    std::vector<std::vector<Link*>>& all_routes,
    std::unordered_set<std::string>& visited,
    int max_depth
) {
    if (current_node == destination) {
        if (!current_route.empty()) {
            all_routes.push_back(current_route);
        }
        return;
    }
    if (max_depth <= 0) return;
    visited.insert(current_node);

    for (const auto& next : graph[current_node]) {
        const std::string& next_node = next.first;
        Link* link = next.second;
        if (visited.find(next_node) == visited.end()) {
            current_route.push_back(link);
            find_all_routes(next_node, destination, graph, current_route, all_routes, visited, max_depth - 1);
            current_route.pop_back();
        }
    }
    visited.erase(current_node);
}

// �ùķ��̼� �Լ� (������ ����)
void simulate_agent_movements(std::vector<Agent>& agents, std::vector<Link*>& all_links) {
    const int MAX_ITERATIONS = 100; // ���� ���� ������ ���� �ִ� �ݺ� Ƚ��

    // ���� ���� �ð��� �����ϴ� ���� �ʱ�ȭ
    std::vector<double> previous_travel_times(agents.size(), std::numeric_limits<double>::max());

    bool converged = false;
    int iteration = 0;

    for (auto& agent : agents)
    {
        agent.set_previous_travel_times(0.0);
    }

    while (!converged && iteration < MAX_ITERATIONS) {
        ++iteration;

        // ��ũ�� �ð� ���� �ʱ�ȭ
        for (auto& link : all_links) {
            link->reset_time_slots();
        }

        // ������Ʈ���� ���� �ð� ���
        for (size_t agent_id = 0; agent_id < agents.size(); ++agent_id) {
            Agent& agent = agents[agent_id];
            agent.reset_travel_info();

            double current_time = agent.get_start_time();

            for (auto& link : agent.get_current_route()) {
                double link_free_flow_time = link->get_free_flow_time();

                int link_start_time = static_cast<int>(std::floor(current_time));
                int link_end_time = static_cast<int>(std::ceil(current_time + link_free_flow_time));

                // ��ũ�� �ش� �ð��뿡 ������Ʈ �߰�
                for (int t = link_start_time; t < link_end_time; ++t) {
                    int slot = t - SIMULATION_START_TIME;
                    if (slot >= 0 && slot < link->get_time_slot_size()) {
                        link->add_agent_to_time_slot(slot, static_cast<int>(agent_id));
                    }
                }

                // ��ũ���� ������Ʈ�� ������ �ٸ� ������Ʈ ���� ��� ���
                double average_traffic = link->calculate_average_congestion(link_start_time, link_end_time);

                // ȥ�⵵�� ����� ��ũ�� ���� ���� �ð� ���
                double volume_capacity_ratio = average_traffic / link->get_capacity();
                double travel_time_on_link = 0.0;

                if (link->get_type() == "Railway") {
                    double denom = link->get_capacity() - average_traffic;
                    if (denom <= 0) denom = 0.1;
                    travel_time_on_link = link->get_free_flow_time() * (1 + link->get_congestion_factor() * (average_traffic / denom));
                }
                else {
                    //volume_capacity_ratio = std::min(volume_capacity_ratio, 2.0); // �ִ밪 ����
                    travel_time_on_link = link->get_free_flow_time() * (1 + link->get_congestion_factor() * std::pow(volume_capacity_ratio, link->get_exponent()));
                }

                // ���� �ð� ������Ʈ
                agent.update_travel_time(travel_time_on_link);
                current_time += travel_time_on_link;
            }

            // ������Ʈ�� ���� �ð� ���
            agent.set_arrival_time(agent.get_start_time() + agent.get_travel_time());
        }

        converged = true;
        for (auto& agent : agents)
        {
            if (std::abs(agent.get_travel_time() - agent.get_previous_travel_times()) > 1e-6)  //TIME_STEP / 2)
                converged = false;
            agent.set_previous_travel_times(agent.get_travel_time());
        }
    }

    // ���� �Ŀ� �� ��ũ�� ��ü ��� ���뷮 ��� (�ʿ��� ���)
    for (auto& link : all_links) {
        link->calculate_overall_average_traffic();
    }
}

// ��ƿ��Ƽ ��� �Լ�
void calculate_agent_utilities(std::vector<Agent>& agents,int mode) {
    for (auto& agent : agents) {
        agent.calc_agent_utility(mode);
    }
}

// CSV ��� �Լ�
void log_agent_utilities(std::vector<Agent>& agents, std::ofstream& agent_utility_file, int iter) {
    for (size_t agent_id = 0; agent_id < agents.size(); ++agent_id) {
        Agent& agent = agents[agent_id];
        agent.agent_log_agent_utilitie(agent_id, agent_utility_file, iter);
    }
}

void reassign_agents(
    std::vector<Agent>& agents,
    std::mt19937& gen,
    std::uniform_real_distribution<>& start_time_dist,
    std::uniform_int_distribution<>& location_dist,
    std::unordered_map<std::string, std::vector<std::pair<std::string, Link*>>>& graph,
    std::vector<Location*>& origins, // origins ���͸� ����
    std::vector<Link*>& all_links,
    int mode
) {
    // 1. ������Ʈ�� �� ��ƿ��Ƽ�� �������� �����մϴ�.
    std::vector<std::pair<double, int>> agent_utilities;
    for (size_t i = 0; i < agents.size(); ++i) {
        agent_utilities.push_back({ agents[i].get_prev_utility(), static_cast<int>(i) });
        // ������ϱ� ���� ��� ������Ʈ�� ������ ������ŵ�ϴ�.
        //agents[i].add_habit();
        agents[i].set_isChangeHabit(false);
    }

    std::sort(agent_utilities.begin(), agent_utilities.end());

    // 2. ���� 10% ������Ʈ�� �����մϴ�.
    int num_reassign = static_cast<int>(agents.size() * 0.1);

    // 3. ���õ� ������Ʈ���� ������մϴ�.
    for (int i = 0; i < num_reassign; ++i) {
        int agent_index = agent_utilities[i].second;
        Agent& agent = agents[agent_index];

        // ���� ��������� ������Ʈ ����
        agent.get_home()->remove_agent();

        // ���ο� ������� �����ϰ� ���� (�뷮 Ȯ��)
        Location* new_home = nullptr;
        std::vector<Location*> possible_homes = origins; // ������ ����� ���

        // ������ ����� �߿��� �뷮�� �ִ� ���� ã���ϴ�.
        std::shuffle(possible_homes.begin(), possible_homes.end(), gen);
        for (Location* loc : possible_homes) {
            if (loc->has_capacity()) {
                new_home = loc;
                break;
            }
        }

        if (!new_home) {
            // ��� ������� �뷮�� ���� �� ���, �� ������Ʈ�� ��������� �ʽ��ϴ�.
            agent.get_home()->add_agent(); // ���� ������� �ٽ� �߰�
            continue;
        }

        agent.set_home(new_home);
        agent.get_home()->add_agent();

        // ������ ��� ������Ʈ
        std::vector<std::vector<Link*>> possible_routes;
        std::vector<Link*> current_route;
        std::unordered_set<std::string> visited;
        int max_depth = 10;
        find_all_routes(agent.get_home()->get_name(), agent.get_work()->get_name(), graph, current_route, possible_routes, visited, max_depth);

        if (possible_routes.empty()) {
            // ������ ��ΰ� ���� ���, ��������� �ʽ��ϴ�.
            //agent.get_home()->remove_agent();
            //agent.get_home()->add_agent(); // ���� ������� �ٽ� �߰�
            continue;
        }

        // ��θ� �����ϰ� �����մϴ�.
        std::uniform_int_distribution<> route_dist(0, possible_routes.size() - 1);
        if (agent.get_current_route() != possible_routes[route_dist(gen)])
        {
            agent.set_current_route(possible_routes[route_dist(gen)]);
            /*      if (agent.get_habit() > 5)
                      agent.set_habit(4);
                  else if (agent.get_habit() > 1)
                      agent.down_upate_habit(2);
                  else
                      agent.set_habit(0);*/

            agent.set_isChangeHabit(true);
        }
        agent.set_start_time(start_time_dist(gen) * 60);
    }

    //��ó�� ������ ��ƿ��Ƽ ������ ��ó���ϱ� ���ؼ� �ִ� ��
    for (size_t i = 0; i < agents.size(); ++i)
    {
        if (mode == 1)
        {
            if (agents[i].get_isChangeHabit())
            {
                if(agents[i].get_habit() <= 5&& agents[i].get_habit() > 0)
                    agents[i].down_upate_habit(1);
            }
            else
            {
                if (agents[i].get_habit() < 5 && agents[i].get_habit() >= 0)
                    agents[i].add_habit();
            }
        }
        else if (mode == 2)
        {
            if (agents[i].get_isChangeHabit())
            {
                    agents[i].set_habit(0);
            }
            else
            {
                if (agents[i].get_habit() < 5 && agents[i].get_habit() >= 0)
                    agents[i].add_habit();
            }
        }
        else if (mode == 3)
        {
            if (agents[i].get_isChangeHabit())
            {
                if (agents[i].get_habit() < 5 && agents[i].get_habit() >= 0)
                    agents[i].add_habit();
            }
            else
            {
                agents[i].set_habit(0);
            }
        }
    }
}

int main() {
    std::ofstream agent_utility_file("agent_utility.csv");
    agent_utility_file << "Iteration,Agent ID,Home,Work,Start Time,Route,Travel Time,SDE (Early Penalty),SDL (Late Penalty),Travel Time Utility,Punctuality Utility,Location Utility,Habit Utility,Total Utility\n";

    std::random_device rd;
    std::mt19937 gen(rd());

    // ������ ���� �ʱ�ȭ
    std::unordered_map<std::string, Location*> locations;
    std::unordered_map<std::string, Link*> links;
    std::vector<Link*> all_links;
    std::unordered_map<std::string, std::vector<std::pair<std::string, Link*>>> graph;

    // �ݺ�(iteration)�� ��ũ ������Ʈ ������ ������ ����ü�� ����
    struct LinkUpdateInfo {
        int iteration;
        std::string link_name;
        std::string link_type;
        double free_flow_time;
        double congestion_factor;
        int capacity;
        double exponent;
        std::string from_node;
        std::string to_node;
        bool is_active;
        bool is_new_link; // true�̸� ���ο� ��ũ �߰�
    };
    std::vector<LinkUpdateInfo> link_updates;

    // �ݺ�(iteration)�� ������Ʈ ������Ʈ ������ ������ ����ü�� ����
    struct AgentUpdateInfo {
        int iteration;
        int habit;
        double beta_k;
        bool is_active;
    };
    std::vector<AgentUpdateInfo> agent_updates;

    struct AgentAddInfo {
        int iteration;
        int count;
        std::string home;
        std::string work;
        bool is_active;
    };
    std::vector<AgentAddInfo> agent_adds;

    // �� �ݺ� Ƚ���� ������ ����
    int TOTAL_ITERATIONS = 2500; // �⺻��
    int TOTAL_AGENTS = 1000; // �⺻��

    // �Է� ���� ����
    std::ifstream infile("input.txt");
    if (!infile) {
        std::cerr << "input.txt ������ �� �� �����ϴ�." << std::endl;
        return 1;
    }

    std::string line;
    // �� �ݺ� Ƚ�� �� ������Ʈ �� �б�
    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty() ) {
            // �� ���̸� ���� �������� �̵�
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "TotalIterations") {
            iss >> TOTAL_ITERATIONS;
        }
        else if (type == "TotalAgents") {
            iss >> TOTAL_AGENTS;
        }
    }

    // Location �б�
    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty()) {
            // �� ���̸� ���� �������� �̵�
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "Location") {
            std::string name;
            int capacity;
            iss >> name >> capacity;
            // Location ���� �� ����
            Location* loc = new Location(name, capacity);
            locations[name] = loc;
        }
    }

    // Link �б�
    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty()) {
            // �� ���̸� ���� �������� �̵�
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "Link") {
            std::string name, link_type, from_node, to_node;
            double free_flow_time, congestion_factor, exponent;
            int capacity;
            iss >> name >> link_type >> free_flow_time >> capacity >> congestion_factor >> exponent >> from_node >> to_node;
            // Link ���� �� ����
            Link* link = new Link(name, link_type, free_flow_time, capacity, congestion_factor, exponent);
            links[name] = link;
            all_links.push_back(link);
            // �׷��� ����
            graph[from_node].push_back({ to_node, link });
            // to_node�� �׷����� ������ �߰�
            if (graph.find(to_node) == graph.end()) {
                graph[to_node] = {};
            }
        }
    }

    // ������Ʈ ���� ���� �б�
    struct AgentInfo {
        std::string home_name;
        std::string work_name;
        double PAT;
    };
    std::vector<AgentInfo> agent_types;

    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty()) {
            // �� ���̸� ���� �������� �̵�
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "Agent") {
            std::string home_name, work_name;
            double PAT;
            iss >> home_name >> work_name >> PAT;
            agent_types.push_back({ home_name, work_name, PAT });
        }
    }

    //��� ������Ʈ
    int mode;
    double habit_parameter;
    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty()) {
            // �� ���̸� ���� �������� �̵�
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "Mode") {
            iss >> mode >> habit_parameter;
            if (mode == 1)
                habit_parameter = abs(habit_parameter);
            else if (mode == 2)
                habit_parameter = abs(habit_parameter);
            else if (mode == 3)
                habit_parameter = -abs(habit_parameter);
            else
                habit_parameter = 0.0;
        }
    }

    // ��ũ ������Ʈ ���� �б�
    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty()) {
            // �� ���̸� ����
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "LinkUpdate") {
            int iteration;
            std::string link_name, link_type, active_str;
            double free_flow_time, congestion_factor, exponent;
            int capacity;
            std::string from_node, to_node;
            bool is_active;
            iss >> iteration >> link_name >> link_type >>free_flow_time >> capacity >> congestion_factor >> exponent >> from_node >> to_node >> active_str;
            is_active = (active_str == "true");
            // ���� ��ũ���� Ȯ��
            bool is_new_link = (links.find(link_name) == links.end());
            link_updates.push_back({ iteration, link_name,link_type, free_flow_time, congestion_factor, capacity, exponent, from_node, to_node, is_active, is_new_link });
        }
    }

    // ������Ʈ ������Ʈ ���� �б�
    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty()) {
            // �� ���̸� ����
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "AgentUpdate") {
            int iteration;
            std::string active_str;
            int habit;
            double beta_k;
            iss >> iteration >> habit>>beta_k >> active_str;
            bool is_active = (active_str == "true");
            agent_updates.push_back({ iteration, habit, beta_k, is_active });
        }
    }

    // ��ũ ������Ʈ ���� �б�
    while (std::getline(infile, line)) {
        if (line[0] == '#')
            continue;
        if (line.empty()) {
            // �� ���̸� ����
            break;
        }
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "AgentAdd") {
            // AgentAdd 1000 400 D A �� ���� ����
            int iteration, count;
            std::string home, work, active_str;
            iss >> iteration >> count >> home >> work>> active_str;
            bool is_active = (active_str == "true");
            agent_adds.push_back({ iteration, count, home, work,is_active });
        }
    }

    infile.close();

    // ����� ��� ���� (A�� �������̹Ƿ� ����)
    std::vector<Location*> origins;
    for (auto& loc_pair : locations) {
        if (loc_pair.first != "A") { // "A"�� �������̹Ƿ� ����
            origins.push_back(loc_pair.second);
        }
    }

    // ������Ʈ ����
    std::vector<Agent> agents;
    std::uniform_real_distribution<> start_time_dist(7.0, 9.0);
    std::uniform_int_distribution<> agent_type_dist(0, agent_types.size() - 1);

    for (int i = 0; i < TOTAL_AGENTS; ++i) {
        // ������ ������Ʈ ���� �� �ϳ��� �����ϰ� ����
        int type_index = agent_type_dist(gen);
        AgentInfo& agent_info = agent_types[type_index];

        Location* home = locations[agent_info.home_name];
        Location* work = locations[agent_info.work_name];

        // home�� "A"�� ��� �������� ����
        if (home == nullptr || work == nullptr || home->get_name() == "A") {
            continue;
        }

        if (home->has_capacity()) {
            double random_start_time = start_time_dist(gen);
            home->add_agent();
            // �ش� ��������� ������ ��� ã��
            std::vector<std::vector<Link*>> possible_routes;
            std::vector<Link*> current_route;
            std::unordered_set<std::string> visited;
            int max_depth = 10;
            find_all_routes(home->get_name(), work->get_name(), graph, current_route, possible_routes, visited, max_depth);
            if (possible_routes.empty()) {
                // ������ ��ΰ� ���� ���, ������Ʈ�� �������� ����
                home->remove_agent();
                continue;
            }
            agents.emplace_back(home, work, possible_routes, origins, agent_info.PAT, random_start_time, gen, habit_parameter);
        }
    }

    // ������Ʈ �� ���
    std::cout << "Total Agents: " << agents.size() << std::endl;

    // Location�� Link�� ����Ͽ� �ùķ��̼� ����
    std::uniform_int_distribution<> location_dist(1, locations.size());

    for (int iter = 0; iter < TOTAL_ITERATIONS; ++iter) {

        // AgentAdd ���� �κ�
        for (const auto& agent_add : agent_adds) {
            if (agent_add.iteration == iter && agent_add.is_active) {
                Location* home = locations[agent_add.home];
                Location* work = locations[agent_add.work];
                if (home && work && home->get_name() != "A") {
                    for (int i = 0; i < agent_add.count; ++i) {
                        if (home->has_capacity()) {
                            double random_start_time = start_time_dist(gen);
                            home->add_agent();
                            std::vector<std::vector<Link*>> possible_routes;
                            std::vector<Link*> current_route;
                            std::unordered_set<std::string> visited;
                            int max_depth = 10;
                            // ���⼭�� PAT�� 8.0���� ���� (�ʿ�� ���� ����)
                            double PAT = 8.0;
                            find_all_routes(home->get_name(), work->get_name(), graph, current_route, possible_routes, visited, max_depth);
                            if (possible_routes.empty()) {
                                home->remove_agent();
                                continue;
                            }
                            agents.emplace_back(home, work, possible_routes, origins, PAT, random_start_time, gen, habit_parameter);
                        }
                        else {
                            break;
                        }
                    }
                }
            }
        }

        // Ư�� �ݺ����� ������Ʈ ������Ʈ ó��
        for (const auto& agent_update : agent_updates) {
            if (agent_update.iteration == iter && agent_update.is_active) {
                // ��� ������Ʈ�� habit�� beta_k�� 0���� ����
                for (auto& agent : agents) {
                    agent.set_habit(agent_update.habit);
                    agent.set_beta_k(agent_update.beta_k);
                }
            }
        }

        // Ư�� �ݺ����� ��ũ ������Ʈ
        for (const auto& update : link_updates) {
            if (update.iteration == iter && update.is_active) {
                if (update.is_new_link) {
                    // ���ο� ��ũ �߰�
                    Link* new_link = new Link(update.link_name, update.link_type, update.free_flow_time, update.capacity, update.congestion_factor, update.exponent);
                    links[update.link_name] = new_link;
                    all_links.push_back(new_link);
                    // �׷����� �߰�
                    graph[update.from_node].push_back({ update.to_node, new_link });
                    // to_node�� �׷����� ������ �߰�
                    if (graph.find(update.to_node) == graph.end()) {
                        graph[update.to_node] = {};
                    }

                    // ������Ʈ�� ������ ��θ� ������Ʈ�ϱ� ���� ��ü ��θ� �ٽ� ���
                    for (auto& agent : agents) {
                        std::vector<std::vector<Link*>> possible_routes;
                        std::vector<Link*> current_route;
                        std::unordered_set<std::string> visited;
                        int max_depth = 10;
                        find_all_routes(agent.get_home()->get_name(), agent.get_work()->get_name(), graph, current_route, possible_routes, visited, max_depth);
                        if (!possible_routes.empty()) {
                            agent.set_current_route(possible_routes[0]); // ������ ù ��° ��η� ����
                        }
                    }
                }
                else {
                    // ���� ��ũ ������Ʈ
                    Link* link = links[update.link_name];
                    if (link) {
                        link->set_free_flow_time(update.free_flow_time);
                        link->set_congestion_factor(update.congestion_factor);
                        link->set_capacity(update.capacity);
                        link->set_exponent(update.exponent);
                    }
                }
            }
        }

        // �ùķ��̼� �ܰ�
        simulate_agent_movements(agents, all_links);

        // ��ƿ��Ƽ ��� �ܰ�
        calculate_agent_utilities(agents,mode);

        // CSV ��� �ܰ�
        log_agent_utilities(agents, agent_utility_file, iter);

        // �� ��ƿ��Ƽ ���
        double total_utility = 0.0;
        for (auto& agent : agents) {
            total_utility += agent.get_prev_utility();
        }

        // �ܼ� ���
        std::cout << "Iteration " << iter << " - Total Utility: " << total_utility << std::endl;

        // ������Ʈ ����� �ܰ�
        reassign_agents(agents, gen, start_time_dist, location_dist, graph, origins, all_links, mode);
    }

    agent_utility_file.close();

    // ���� �޸� ����
    for (auto& pair : locations) {
        delete pair.second;
    }
    for (auto& pair : links) {
        delete pair.second;
    }

    return 0;
}