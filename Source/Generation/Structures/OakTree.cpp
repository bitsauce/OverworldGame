#include "OakTree.h"

struct LeafPoint
{
    LeafPoint(int x, int y, int r) :
		x(x),
		y(y),
		r(r)
	{
	}

    int x, y, r;
};

struct Branch
{
    Branch(int x, int y, int dir, float chance) :
		x(x),
		y(y),
		dir(dir),
		chance(chance)
	{
	}

    int x, y, dir;
	float chance;
};

void OakTree::place(WorldGenerator *worldGenerator)
{
	Random rand = worldGenerator->getRandom();
	rand.setSeed(rand.getSeed() + m_x);

    float chance1 = rand.nextInt() % 2 == 0 ? 1.0f : 0.0f;
	float chance2 = 1.0f - chance1;
    int stemDir = rand.nextInt() % 2 == 0 ? 1 : -1;
    int stemHeight = 20 + rand.nextInt() % 11;
    list<LeafPoint> leafPoints;

	for(int i = -1; i <= 1; ++i)
	{
		worldGenerator->setBlockAt(m_x + i, m_y, TERRAIN_LAYER_BACK, BLOCK_OAK_WOOD);
	}

    for(int y = 0; y < stemHeight; ++y)
    {
		worldGenerator->setBlockAt(m_x, m_y - y, TERRAIN_LAYER_BACK, BLOCK_OAK_WOOD);

        if(y > 3 && y < stemHeight * 0.70f)
        {
            // Create branches
			double branchChance = rand.nextDouble();
            stack<Branch> branches;
			if(branchChance < chance1)
			{
                branches.push(Branch(m_x, m_y - y,  1, 0.25f));
				branches.push(Branch(m_x, m_y - y, -1, 0.25f));
				chance1 = 0.0f;
			}
			else
			{
                chance1 += 0.05f;
			}
				
			/*if(branchChance < chance2)
            {
                branches.push(Branch(m_x, m_y - y, -1));
				chance2 = 0.0f;
            }
			else
			{
                chance2 += 0.05f;
			}*/
			
			while(!branches.empty())
            {
				Branch branch = branches.top();
				branches.pop();

                double inclination = (0.2f + rand.nextDouble() * 0.8f);// * (1.0f - stemHeight / 35.0f);
                int branchX = branch.x, branchY = branch.y;
                int branchLength = (int) (6 * stemHeight / 30.0f * (0.5f + rand.nextDouble() * 0.5f));
                for (int i = 0; i <= branchLength; i++)
                {
                    int dy = (int) (i * inclination);
                    for (int y1 = 0; y1 <= dy; y1++)
					{
						worldGenerator->setBlockAt(branchX, branchY - y1, TERRAIN_LAYER_BACK, BLOCK_OAK_WOOD);
					}

					branchX += branch.dir;
                    branchY -= dy;

                    if(rand.nextDouble() < branch.chance)
                    {
						branches.push(Branch(branchX, branchY, branch.dir, branch.chance * 0.25f));
                    }
                }
				leafPoints.push_back(LeafPoint(branchX, branchY, branchLength));
            }
        }
    }
	leafPoints.push_back(LeafPoint(m_x, m_y - stemHeight, (int) (stemHeight * 0.2)));

    for(LeafPoint p : leafPoints)
    {
        for (int y1 = -p.r; y1 <= p.r; y1++)
        {
            for (int x1 = -p.r; x1 <= p.r; x1++)
            {
                if (sqrt(x1 * x1 + y1 * y1) > p.r) continue;
				worldGenerator->setBlockAt(p.x + x1, p.y + y1, TERRAIN_LAYER_FRONT, BLOCK_OAK_LEAVES);
            }
        }
    }
}