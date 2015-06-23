#include "OakTree.h"
//>REMOVE ME<
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
    Branch(int x, int y, int dir) :
		x(x),
		y(y),
		dir(dir)
	{
	}

    int x, y, dir;
};

void OakTree::place(WorldGenerator *worldGenerator, StructurePlacer *structPlacer)
{
	Random rand = worldGenerator->getRandom();
	rand.setSeed(rand.getSeed() + m_x);

    float chance1 = rand.nextInt() % 2 == 0 ? 1.0f : 0.0f;
	float chance2 = 1.0f - chance1;
    int stemDir = rand.nextInt() % 2 == 0 ? 1 : -1;
    int stemHeight = 20 + rand.nextInt() % 11;
    list<LeafPoint> leafPoints;
    for(int y = 0; y < stemHeight; ++y)
    {
		if(rand.nextDouble() < (y / (float) stemHeight) * 0.2f)
        {
            m_x += stemDir;
        }

		float w = 2 * (1.0f - pow(y / (float) stemHeight, 2.0f)) * 0.5f;
		for(float f = -w; f <= w; f += 1.0f)
		{
			structPlacer->setBlockAt(m_x + floor(f), m_y - y, TERRAIN_LAYER_BACK, BLOCK_WOOD_OAK);
		}

        if(y > 3 && y < stemHeight * 0.70f)
        {
            // Create branches
			double branchChance = rand.nextDouble();
            stack<Branch> branches;
			if(branchChance < chance1)
			{
                branches.push(Branch(m_x, m_y - y, 1));
				chance1 = 0.0f;
			}
			else
			{
                chance1 += 0.05f;
			}
				
			if(branchChance < chance2)
            {
                branches.push(Branch(m_x, m_y - y, -1));
				chance2 = 0.0f;
            }
			else
			{
                chance2 += 0.05f;
			}
			
            float chance = 0.5f;
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
						structPlacer->setBlockAt(branchX, branchY - y1, TERRAIN_LAYER_BACK, BLOCK_WOOD_OAK);
					}

					branchX += branch.dir;
                    branchY -= dy;

                    if(rand.nextDouble() < chance)
                    {
						branches.push(Branch(branchX, branchY, branch.dir));
                        chance *= 0.25f;
                    }
                }
				leafPoints.push_back(LeafPoint(branchX, branchY, branchLength));
            }
            chance = 0.0f;
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
				structPlacer->setBlockAt(p.x + x1, p.y + y1, TERRAIN_LAYER_FRONT, BLOCK_LEAF);
            }
        }
    }
}