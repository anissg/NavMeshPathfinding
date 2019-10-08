#include "Map.h"

Map::Map() : points_list(), complete_path(false), current_point(), follow_point_added(false), index_follow_point(0)
{
}

Map::~Map()
{
}

void Map::WindowProcedure(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (!complete_path)
    {
        POINTS ptsBegin;

        current_point.x = GET_X_LPARAM(lParam);
        current_point.y = SCREEN_HEIGHT - GET_Y_LPARAM(lParam);

        if (current_point.x < 250 || current_point.y < 0 || current_point.y > SCREEN_HEIGHT)
        {
            if (current_point.x < 250) current_point.x = 250;
            if (current_point.y < 0)  current_point.y = 0;
            if (points_list.size() > 0)
            {
                if (follow_point_added == (points_list.size() - 1))
                {
                    points_list.pop_back();
                    follow_point_added = false;
                }
            }
        }
        else
        {
            if (!follow_point_added)
            {
                points_list.push_back(current_point);
                index_follow_point = points_list.size() - 1;
                follow_point_added = true;
            }
            else
            {
                points_list[index_follow_point].x = current_point.x;
                points_list[index_follow_point].y = current_point.y;
            }

            for (int i = 0; i < points_list.size() - 1; i++)
            {
                Point2 p2;
                p2.x = points_list[i].x;
                p2.y = points_list[i].y;
                float d = distance(p2, current_point);
                if (d < 20)
                {
                    points_list[index_follow_point].x = p2.x;
                    points_list[index_follow_point].y = p2.y;
                    current_point.x = points_list[0].x;
                    current_point.y = points_list[0].y;
                }
            }

            switch (msg)
            {
            case WM_LBUTTONDOWN:
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);
                ptsBegin = MAKEPOINTS(lParam);
                ptsBegin.y = (SCREEN_HEIGHT - ptsBegin.y);
                Point2 p;
                p.x = ptsBegin.x;
                p.y = ptsBegin.y;

                for (int i = 0; i < points_list.size() - 1; i++)
                {
                    Point2 p2;
                    p2.x = points_list[i].x;
                    p2.y = points_list[i].y;
                    float fd = distance(p, p2);
                    if (fd < 20)
                    {
                        return;
                    }
                }
                if (!complete_path)
                {
                    points_list[index_follow_point].x = p.x;
                    points_list[index_follow_point].y = p.y;
                    follow_point_added = false;
                }
                break;
            }
        }
    }
}

float Map::distance(Point2 p1, Point2 p2)
{
    return sqrtf(powf((float)p2.x - (float)p1.x, 2) + powf((float)p2.y - (float)p1.y, 2));
}

bool Map::complete()
{
    return complete_path;
}

void Map::set_complete(bool complete)
{
    complete_path = complete;
    if (complete)
        points_list.pop_back();
}

std::vector<Point2> Map::get_points_list()
{
    return points_list;
}

void Map::set_points_list(std::vector<Point2> points_list)
{
    this->points_list = points_list;
}

std::vector<obstacle> Map::get_obstacles(int demo_map)
{
    std::vector<obstacle> obstacles;
    obstacle obst;

    switch (demo_map)
    {
    case 0:
        obst.push_back(Point2(change_x(581), change_y(709)));
        obst.push_back(Point2(change_x(581 + 140), change_y(709)));
        obst.push_back(Point2(change_x(581 + 140), change_y(709 + 88)));
        obst.push_back(Point2(change_x(581), change_y(709 + 88)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(711), change_y(531)));
        obst.push_back(Point2(change_x(711 + 140), change_y(531)));
        obst.push_back(Point2(change_x(711 + 140), change_y(531 + 88)));
        obst.push_back(Point2(change_x(711), change_y(531 + 88)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(572), change_y(312)));
        obst.push_back(Point2(change_x(572 + 140), change_y(312)));
        obst.push_back(Point2(change_x(572 + 140), change_y(312 + 88)));
        obst.push_back(Point2(change_x(572), change_y(312 + 88)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(954), change_y(658)));
        obst.push_back(Point2(change_x(954 + 70), change_y(658)));
        obst.push_back(Point2(change_x(954 + 70), change_y(658 + 88)));
        obst.push_back(Point2(change_x(954), change_y(658 + 88)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(483), change_y(845)));
        obst.push_back(Point2(change_x(483 + 64), change_y(845)));
        obst.push_back(Point2(change_x(483 + 64), change_y(845 + 64)));
        obst.push_back(Point2(change_x(483), change_y(845 + 64)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(1284), change_y(338)));
        obst.push_back(Point2(change_x(1284 + 140), change_y(338)));
        obst.push_back(Point2(change_x(1284 + 140), change_y(338 + 88)));
        obst.push_back(Point2(change_x(1284), change_y(338 + 88)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(1244), change_y(827)));
        obst.push_back(Point2(change_x(1244 + 32), change_y(827)));
        obst.push_back(Point2(change_x(1244 + 32), change_y(827 + 32)));
        obst.push_back(Point2(change_x(1244), change_y(827 + 32)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(533), change_y(157)));
        obst.push_back(Point2(change_x(533 + 64), change_y(157)));
        obst.push_back(Point2(change_x(533 + 64), change_y(157 + 64)));
        obst.push_back(Point2(change_x(533), change_y(157 + 64)));
        obstacles.push_back(obst);
        obst.clear();
        break;
    case 1:
        obst.push_back(Point2(change_x(527), change_y(738)));
        obst.push_back(Point2(change_x(687), change_y(738)));
        obst.push_back(Point2(change_x(687), change_y(822)));
        obst.push_back(Point2(change_x(527), change_y(822)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(711), change_y(611)));
        obst.push_back(Point2(change_x(839), change_y(611)));
        obst.push_back(Point2(change_x(839), change_y(711)));
        obst.push_back(Point2(change_x(711), change_y(711)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(879), change_y(483)));
        obst.push_back(Point2(change_x(1039), change_y(483)));
        obst.push_back(Point2(change_x(1039), change_y(567)));
        obst.push_back(Point2(change_x(879), change_y(567)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(1092), change_y(603)));
        obst.push_back(Point2(change_x(1220), change_y(603)));
        obst.push_back(Point2(change_x(1220), change_y(703)));
        obst.push_back(Point2(change_x(1092), change_y(703)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(1263), change_y(741)));
        obst.push_back(Point2(change_x(1423), change_y(741)));
        obst.push_back(Point2(change_x(1423), change_y(825)));
        obst.push_back(Point2(change_x(1263), change_y(825)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(605), change_y(392)));
        obst.push_back(Point2(change_x(669), change_y(392)));
        obst.push_back(Point2(change_x(669), change_y(472)));
        obst.push_back(Point2(change_x(605), change_y(472)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(714), change_y(306)));
        obst.push_back(Point2(change_x(778), change_y(306)));
        obst.push_back(Point2(change_x(778), change_y(386)));
        obst.push_back(Point2(change_x(714), change_y(386)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(900), change_y(253)));
        obst.push_back(Point2(change_x(964), change_y(253)));
        obst.push_back(Point2(change_x(964), change_y(333)));
        obst.push_back(Point2(change_x(900), change_y(333)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(1078), change_y(343)));
        obst.push_back(Point2(change_x(1142), change_y(343)));
        obst.push_back(Point2(change_x(1142), change_y(423)));
        obst.push_back(Point2(change_x(1078), change_y(423)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(1242), change_y(449)));
        obst.push_back(Point2(change_x(1306), change_y(449)));
        obst.push_back(Point2(change_x(1306), change_y(529)));
        obst.push_back(Point2(change_x(1242), change_y(529)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(546), change_y(183)));
        obst.push_back(Point2(change_x(610), change_y(183)));
        obst.push_back(Point2(change_x(610), change_y(263)));
        obst.push_back(Point2(change_x(546), change_y(263)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(1303), change_y(182)));
        obst.push_back(Point2(change_x(1367), change_y(182)));
        obst.push_back(Point2(change_x(1367), change_y(262)));
        obst.push_back(Point2(change_x(1303), change_y(262)));
        obstacles.push_back(obst);
        obst.clear();
        break;
    case 2:
        obst.push_back(Point2(change_x(480), change_y(876)));
        obst.push_back(Point2(change_x(544), change_y(876)));
        obst.push_back(Point2(change_x(544), change_y(956)));
        obst.push_back(Point2(change_x(480), change_y(956)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(380), change_y(821)));
        obst.push_back(Point2(change_x(444), change_y(821)));
        obst.push_back(Point2(change_x(444), change_y(901)));
        obst.push_back(Point2(change_x(380), change_y(901)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(455), change_y(712)));
        obst.push_back(Point2(change_x(519), change_y(712)));
        obst.push_back(Point2(change_x(519), change_y(792)));
        obst.push_back(Point2(change_x(455), change_y(792)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(568), change_y(688)));
        obst.push_back(Point2(change_x(632), change_y(688)));
        obst.push_back(Point2(change_x(632), change_y(768)));
        obst.push_back(Point2(change_x(568), change_y(768)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(538), change_y(190)));
        obst.push_back(Point2(change_x(698), change_y(190)));
        obst.push_back(Point2(change_x(698), change_y(274)));
        obst.push_back(Point2(change_x(538), change_y(274)));
        obstacles.push_back(obst);
        obst.clear();
        obst.push_back(Point2(change_x(726), change_y(189)));
        obst.push_back(Point2(change_x(854), change_y(189)));
        obst.push_back(Point2(change_x(854), change_y(289)));
        obst.push_back(Point2(change_x(726), change_y(289)));
        obstacles.push_back(obst);
        obst.clear();

        break;
    }

    return obstacles;
}

void Map::set_demo_map(int demo_map)
{
    points_list.clear();
    switch (demo_map)
    {
    case 0:
        points_list.push_back(Point2(change_x(431), change_y(973)));
        points_list.push_back(Point2(change_x(387), change_y(956)));
        points_list.push_back(Point2(change_x(370), change_y(904)));
        points_list.push_back(Point2(change_x(391), change_y(812)));
        points_list.push_back(Point2(change_x(449), change_y(696)));
        points_list.push_back(Point2(change_x(512), change_y(623)));
        points_list.push_back(Point2(change_x(541), change_y(570)));
        points_list.push_back(Point2(change_x(521), change_y(474)));
        points_list.push_back(Point2(change_x(447), change_y(390)));
        points_list.push_back(Point2(change_x(397), change_y(324)));
        points_list.push_back(Point2(change_x(366), change_y(278)));
        points_list.push_back(Point2(change_x(347), change_y(195)));
        points_list.push_back(Point2(change_x(390), change_y(88)));
        points_list.push_back(Point2(change_x(703), change_y(42)));
        points_list.push_back(Point2(change_x(762), change_y(105)));
        points_list.push_back(Point2(change_x(828), change_y(203)));
        points_list.push_back(Point2(change_x(859), change_y(312)));
        points_list.push_back(Point2(change_x(915), change_y(388)));
        points_list.push_back(Point2(change_x(980), change_y(439)));
        points_list.push_back(Point2(change_x(1068), change_y(450)));
        points_list.push_back(Point2(change_x(1153), change_y(410)));
        points_list.push_back(Point2(change_x(1194), change_y(361)));
        points_list.push_back(Point2(change_x(1272), change_y(270)));
        points_list.push_back(Point2(change_x(1330), change_y(184)));
        points_list.push_back(Point2(change_x(1394), change_y(104)));
        points_list.push_back(Point2(change_x(1498), change_y(70)));
        points_list.push_back(Point2(change_x(1559), change_y(91)));
        points_list.push_back(Point2(change_x(1593), change_y(152)));
        points_list.push_back(Point2(change_x(1598), change_y(260)));
        points_list.push_back(Point2(change_x(1576), change_y(357)));
        points_list.push_back(Point2(change_x(1552), change_y(423)));
        points_list.push_back(Point2(change_x(1494), change_y(488)));
        points_list.push_back(Point2(change_x(1337), change_y(566)));
        points_list.push_back(Point2(change_x(1304), change_y(608)));
        points_list.push_back(Point2(change_x(1319), change_y(697)));
        points_list.push_back(Point2(change_x(1415), change_y(765)));
        points_list.push_back(Point2(change_x(1512), change_y(863)));
        points_list.push_back(Point2(change_x(1534), change_y(920)));
        points_list.push_back(Point2(change_x(1530), change_y(979)));
        points_list.push_back(Point2(change_x(1437), change_y(1025)));
        points_list.push_back(Point2(change_x(1272), change_y(1032)));
        points_list.push_back(Point2(change_x(1164), change_y(975)));
        points_list.push_back(Point2(change_x(1055), change_y(874)));
        points_list.push_back(Point2(change_x(894), change_y(837)));
        points_list.push_back(Point2(change_x(721), change_y(869)));
        points_list.push_back(Point2(change_x(657), change_y(934)));
        points_list.push_back(Point2(change_x(635), change_y(978)));
        break;
    case 1:
        points_list.push_back(Point2(change_x(491), change_y(975)));
        points_list.push_back(Point2(change_x(443), change_y(973)));
        points_list.push_back(Point2(change_x(376), change_y(909)));
        points_list.push_back(Point2(change_x(359), change_y(801)));
        points_list.push_back(Point2(change_x(359), change_y(715)));
        points_list.push_back(Point2(change_x(357), change_y(606)));
        points_list.push_back(Point2(change_x(350), change_y(511)));
        points_list.push_back(Point2(change_x(339), change_y(393)));
        points_list.push_back(Point2(change_x(345), change_y(297)));
        points_list.push_back(Point2(change_x(410), change_y(164)));
        points_list.push_back(Point2(change_x(570), change_y(76)));
        points_list.push_back(Point2(change_x(802), change_y(49)));
        points_list.push_back(Point2(change_x(978), change_y(53)));
        points_list.push_back(Point2(change_x(1177), change_y(53)));
        points_list.push_back(Point2(change_x(1402), change_y(67)));
        points_list.push_back(Point2(change_x(1521), change_y(127)));
        points_list.push_back(Point2(change_x(1627), change_y(243)));
        points_list.push_back(Point2(change_x(1638), change_y(328)));
        points_list.push_back(Point2(change_x(1634), change_y(459)));
        points_list.push_back(Point2(change_x(1630), change_y(617)));
        points_list.push_back(Point2(change_x(1625), change_y(749)));
        points_list.push_back(Point2(change_x(1608), change_y(858)));
        points_list.push_back(Point2(change_x(1587), change_y(922)));
        points_list.push_back(Point2(change_x(1533), change_y(961)));
        points_list.push_back(Point2(change_x(1431), change_y(968)));
        points_list.push_back(Point2(change_x(1294), change_y(974)));
        points_list.push_back(Point2(change_x(1207), change_y(969)));
        points_list.push_back(Point2(change_x(1043), change_y(970)));
        points_list.push_back(Point2(change_x(894), change_y(971)));
        points_list.push_back(Point2(change_x(680), change_y(985)));
        break;
    case 2:
        points_list.push_back(Point2(change_x(1625), change_y(1016)));
        points_list.push_back(Point2(change_x(1481), change_y(1009)));
        points_list.push_back(Point2(change_x(1073), change_y(1010)));
        points_list.push_back(Point2(change_x(793), change_y(981)));
        points_list.push_back(Point2(change_x(532), change_y(987)));
        points_list.push_back(Point2(change_x(422), change_y(963)));
        points_list.push_back(Point2(change_x(364), change_y(910)));
        points_list.push_back(Point2(change_x(320), change_y(819)));
        points_list.push_back(Point2(change_x(337), change_y(732)));
        points_list.push_back(Point2(change_x(498), change_y(665)));
        points_list.push_back(Point2(change_x(693), change_y(670)));
        points_list.push_back(Point2(change_x(862), change_y(669)));
        points_list.push_back(Point2(change_x(1070), change_y(662)));
        points_list.push_back(Point2(change_x(1199), change_y(644)));
        points_list.push_back(Point2(change_x(1267), change_y(611)));
        points_list.push_back(Point2(change_x(1294), change_y(578)));
        points_list.push_back(Point2(change_x(1303), change_y(504)));
        points_list.push_back(Point2(change_x(1278), change_y(457)));
        points_list.push_back(Point2(change_x(1112), change_y(418)));
        points_list.push_back(Point2(change_x(884), change_y(434)));
        points_list.push_back(Point2(change_x(660), change_y(438)));
        points_list.push_back(Point2(change_x(512), change_y(415)));
        points_list.push_back(Point2(change_x(413), change_y(374)));
        points_list.push_back(Point2(change_x(368), change_y(300)));
        points_list.push_back(Point2(change_x(341), change_y(156)));
        points_list.push_back(Point2(change_x(393), change_y(95)));
        points_list.push_back(Point2(change_x(545), change_y(61)));
        points_list.push_back(Point2(change_x(780), change_y(51)));
        points_list.push_back(Point2(change_x(1113), change_y(60)));
        points_list.push_back(Point2(change_x(1320), change_y(83)));
        points_list.push_back(Point2(change_x(1472), change_y(179)));
        points_list.push_back(Point2(change_x(1541), change_y(301)));
        points_list.push_back(Point2(change_x(1582), change_y(443)));
        points_list.push_back(Point2(change_x(1583), change_y(553)));
        points_list.push_back(Point2(change_x(1572), change_y(652)));
        points_list.push_back(Point2(change_x(1384), change_y(718)));
        points_list.push_back(Point2(change_x(1243), change_y(745)));
        points_list.push_back(Point2(change_x(986), change_y(764)));
        points_list.push_back(Point2(change_x(728), change_y(774)));
        points_list.push_back(Point2(change_x(543), change_y(794)));
        points_list.push_back(Point2(change_x(526), change_y(827)));
        points_list.push_back(Point2(change_x(613), change_y(866)));
        points_list.push_back(Point2(change_x(875), change_y(872)));
        points_list.push_back(Point2(change_x(1208), change_y(865)));
        points_list.push_back(Point2(change_x(1393), change_y(867)));
        points_list.push_back(Point2(change_x(1541), change_y(877)));

        break;
    }
}

std::vector<ITEM> Map::get_obstacles_types(int demo_map)
{
    std::vector<ITEM> elements;
    ITEM it;
    switch (demo_map)
    {
    case 0:
        it.p = Point2(change_x(581), change_y(709));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(711), change_y(531));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(572), change_y(312));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(954), change_y(658));
        it.type = 1;
        elements.push_back(it);

        it.p = Point2(change_x(483), change_y(845));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(1284), change_y(338));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(1244), change_y(827));
        it.type = 3;
        elements.push_back(it);

        it.p = Point2(change_x(533), change_y(157));
        it.type = 2;
        elements.push_back(it);
        break;
    case 1:

        it.p = Point2(change_x(527), change_y(738));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(711), change_y(611));
        it.type = 1;
        elements.push_back(it);

        it.p = Point2(change_x(879), change_y(483));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(1092), change_y(603));
        it.type = 1;
        elements.push_back(it);

        it.p = Point2(change_x(1263), change_y(741));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(605), change_y(392));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(714), change_y(306));
        it.type = 3;
        elements.push_back(it);

        it.p = Point2(change_x(900), change_y(253));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(1078), change_y(343));
        it.type = 3;
        elements.push_back(it);

        it.p = Point2(change_x(1242), change_y(449));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(546), change_y(183));
        it.type = 3;
        elements.push_back(it);

        it.p = Point2(change_x(1303), change_y(182));
        it.type = 2;
        elements.push_back(it);
        break;
    case 2:

        it.p = Point2(change_x(480), change_y(876));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(380), change_y(821));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(455), change_y(712));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(568), change_y(688));
        it.type = 2;
        elements.push_back(it);

        it.p = Point2(change_x(538), change_y(190));
        it.type = 0;
        elements.push_back(it);

        it.p = Point2(change_x(726), change_y(189));
        it.type = 1;
        elements.push_back(it);
        break;
    }
    return elements;
}

int Map::change_x(int val)
{
    return (int)(0.719 * (float)val);
}

int Map::change_y(int val)
{
    return (int)(0.7314 * (float)val);
}
